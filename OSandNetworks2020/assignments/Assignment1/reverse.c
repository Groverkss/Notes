#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

const int BUFFER_SIZE = 81920;

void create_dir(void) {
    int dir_check = mkdir("Assignment", 0700);
    if (dir_check == -1) {
        if (errno != EEXIST) {
            perror("reverse");
            exit(1);
        }
    }
}

int main(int argc, char *argv[]) {
    // Check for arguements
    if (argc != 2) {
        char *arg_error = "Usage: ./a.out <path_of_file_to_be_reversed>\n";
        write(STDERR_FILENO, arg_error, strlen(arg_error));
        exit(1);
    }

    // Make directory and implement error handeling
    create_dir();

    // Open file provided
    int arg1_fd = open(argv[1], O_RDONLY);
    if (arg1_fd == -1) {
        perror("reverse");
        exit(1);
    }

    // Open file for writing
    int wr_fd = open("Assignment/reverse.out",
            O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (wr_fd == -1) {
        perror("reverse");
        exit(1);
    }

    // Setup percentage
    char *cmplt = "Percentage Completion: \n";
    ssize_t nwrote = write(STDOUT_FILENO, cmplt, strlen(cmplt));
    if (nwrote == -1) {
        perror("reverse");
        exit(1);
    }


    char buffer[BUFFER_SIZE + 1], per_out[10];

    off_t current_off = lseek(arg1_fd, 0, SEEK_END);

    long long total_bytes = current_off;
    // Read and write
    while(current_off >= BUFFER_SIZE) {
        // Go back 
        current_off = lseek(arg1_fd, -BUFFER_SIZE, SEEK_CUR);
        if (current_off  == -1) {
            perror("reverse");
            exit(1);
        }

        // Read the contents
        ssize_t nread = read(arg1_fd, buffer, BUFFER_SIZE);
        if (nread == -1) {
            perror("reverse");
            exit(1);
        }

        for (int i = 0; i < BUFFER_SIZE / 2; i++) {
            char tmp = buffer[i];
            buffer[i] = buffer[BUFFER_SIZE - i - 1];
            buffer[BUFFER_SIZE - i - 1] = tmp;
        }

        // Write the contents
        ssize_t nwrote = write(wr_fd, buffer, BUFFER_SIZE);
        if (nwrote == -1) {
            perror("reverse");
            exit(1);
        }

        // Return seek to correct position
        off_t check_off = lseek(arg1_fd, current_off, SEEK_SET);
        if (check_off == -1) {
            perror("reverse");
            exit(1);
        }

        // Calculate percentage
        long long bytes_left = (total_bytes - current_off) * 100;
        long double curr_percent =
            ((long double) bytes_left) / ((long double) total_bytes);
        sprintf(per_out, "\r%0.2Lf", curr_percent);

        // Write percentage
        nwrote = write(STDOUT_FILENO, per_out, strlen(per_out));
        if (nwrote == -1) {
            perror("reverse");
            exit(1);
        }
    }

    if (current_off != 0) {
        // Read the remaining characters
        long long to_read = current_off;
        current_off = lseek(arg1_fd, 0, SEEK_SET);

        // Read the contents
        ssize_t nread = read(arg1_fd, buffer, to_read);
        if (nread == -1) {
            perror("reverse");
            exit(1);
        }

        for (int i = 0; i < to_read / 2; i++) {
            char tmp = buffer[i];
            buffer[i] = buffer[to_read - i - 1];
            buffer[to_read - i - 1] = tmp;
        }

        // Write the contents
        ssize_t nwrote = write(wr_fd, buffer, to_read);
        if (nwrote == -1) {
            perror("reverse");
            exit(1);
        }

        // Calculate percentage
        long long bytes_left = (total_bytes - current_off) * 100;
        long double curr_percent =
            ((long double) bytes_left) / ((long double) total_bytes);
        sprintf(per_out, "\r%0.2Lf", curr_percent);

        // Write percentage                                                               
        nwrote = write(STDOUT_FILENO, per_out, strlen(per_out));        
        if (nwrote == -1) {                                                     
            perror("reverse");                                               
            exit(1);                                                            
        }        
    }

    close(wr_fd);
    close(arg1_fd);
}

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

const int BUFFER_SIZE = 1024;

int main(int argc, char *argv[]) {
    // Check for arguements
    if (argc != 2) {
        char *arg_error = "Usage: ./revese <path_of_file_to_be_reversed>\n";
        write(STDOUT_FILENO, arg_error, strlen(arg_error));
        exit(1);
    }

    // Open file provided
    int arg1_fd = open(argv[1], O_RDONLY);
    if (arg1_fd == -1) {
        perror("reverse");
        exit(1);
    }

    char buffer[BUFFER_SIZE];

    off_t current_off = lseek(arg1_fd, 0, SEEK_END);
    while(current_off >= BUFFER_SIZE) {
        // Go back 
        current_off = lseek(arg1_fd, -BUFFER_SIZE, SEEK_CUR);
        if (nread == -1) {
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
        ssize_t nwrote = write(STDOUT_FILENO, buffer, BUFFER_SIZE);
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
    }

    if (current_off != 0) {
        // Read the remaining characters
        lseek(arg1_fd, 0, SEEK_SET);
        int to_read = current_off;

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
        ssize_t nwrote = write(STDOUT_FILENO, buffer, to_read);
        if (nwrote == -1) {
            perror("reverse");
            exit(1);
        }
    }
}

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

const int BUFFER_SIZE = 512;

void write_out(char *str, int dec, char *filename) {
    char str_write[100];

    sprintf(str_write, "%s%s: %s\n\n", str, filename, dec ? "Yes" : "No");
    int nwrote = write(STDOUT_FILENO, str_write, strlen(str_write));
    if (nwrote == -1) {
        perror("Error");
        exit(1);
    }
}

void print_perm(mode_t perm, char *filename) {
    int dec;

    // User read
    (perm & S_IRUSR) ? (dec = 1) : (dec = 0);
    write_out("User has read permissions on ", dec, filename);

    // User write
    (perm & S_IWUSR) ? (dec = 1) : (dec = 0);
    write_out("User has write permissions on ", dec, filename);

    // User execute
    (perm & S_IXUSR) ? (dec = 1) : (dec = 0);
    write_out("User has execute permissions on ", dec, filename);

    // Group read
    (perm & S_IRGRP) ? (dec = 1) : (dec = 0);
    write_out("Group has read permissions on ", dec, filename);

    // Group write
    (perm & S_IWGRP) ? (dec = 1) : (dec = 0);
    write_out("Group has write permissions on ", dec, filename);

    // Group execute
    (perm & S_IXGRP) ? (dec = 1) : (dec = 0);
    write_out("Group has execute permissions on ", dec, filename);

    // Others read
    (perm & S_IROTH) ? (dec = 1) : (dec = 0);
    write_out("Others has read permissions on ", dec, filename);

    // Others write
    (perm & S_IWOTH) ? (dec = 1) : (dec = 0);
    write_out("Others has write permissions on ", dec, filename);

    // Others execute
    (perm & S_IXOTH) ? (dec = 1) : (dec = 0);
    write_out("Others has execute permissions on ", dec, filename);
}

int compare_strings(char *a, char *b, int len) {
    for (int i = 0; i < len; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

void check_reverse(char *newfile, char* oldfile) {

   // Open files
   int nfile_fd = open(newfile, O_RDONLY);
   if (nfile_fd == -1) {
       perror("Error");
       exit(1);
   }

   int ofile_fd = open(oldfile, O_RDONLY);
   if (ofile_fd == -1) {
       perror("Error");
       exit(1);
   }

   // Move seek to end of both files to check length of files
   off_t nfile_off = lseek(nfile_fd, 0, SEEK_END);
   off_t ofile_off = lseek(ofile_fd, 0, SEEK_END);

   // Check if file lengths are equal
   if (ofile_off != nfile_off) {
       write_out("Wether file contents are reversed in ", 0, "newfile");
       return;
   }

   // Set seek of old file to zero
   ofile_off = lseek(ofile_fd, 0, SEEK_SET);

   char nbuffer[BUFFER_SIZE + 1], obuffer[BUFFER_SIZE + 1]; 

   while(nfile_off >= BUFFER_SIZE) {
       // Go back
       nfile_off = lseek(nfile_fd, -BUFFER_SIZE, SEEK_CUR);

       // Read from newfile
       ssize_t nread = read(nfile_fd, nbuffer, BUFFER_SIZE);
       if (nread == -1) {
           perror("Error");
           exit(1);
       }

       nbuffer[BUFFER_SIZE] = '\0';

       // Read from oldfile
       nread = read(ofile_fd, obuffer, BUFFER_SIZE);
       if (nread == -1) {
           perror("Error");
           exit(1);
       }

       obuffer[BUFFER_SIZE] = '\0';

       // Reverse nfile buffer
       for (int i = 0; i < BUFFER_SIZE / 2; i++) {
           char tmp = nbuffer[i];
           nbuffer[i] = nbuffer[BUFFER_SIZE - i - 1];
           nbuffer[BUFFER_SIZE - i - 1] = tmp;
       }

       // Compare contents of file
       if (!compare_strings(nbuffer, obuffer, BUFFER_SIZE)) {
           write_out("Wether file contents are reversed in ", 0, "newfile");
           return;
       }

       lseek(nfile_fd, nfile_off, SEEK_SET);
   }

   if (nfile_off > 0) {
       // Go back
       lseek(nfile_fd, 0, SEEK_SET);  

       // Read from files
       ssize_t nread = read(nfile_fd, nbuffer, nfile_off);
       if (nread == -1) {
           perror("Error");
           exit(1);
       }

       nbuffer[nfile_off] = '\0';

       nread = read(ofile_fd, obuffer, nfile_off);
       if (nread == -1) {
           perror("Error");
           exit(1);
       }

       obuffer[nfile_off] = '\0';

       // Reverse nfile buffer
       for (int i = 0; i < nfile_off / 2; i++) {
            char tmp = nbuffer[i];
            nbuffer[i] = nbuffer[nfile_off - i - 1];
            nbuffer[nfile_off - i - 1] = tmp;
       }

       if (!compare_strings(nbuffer, obuffer, nfile_off)) {
           write_out("Wether file contents are reversed in ", 0, "newfile");
           return;
       }
   }

   // Report success
   write_out("Wether file contents are reversed in ", 1, "newfile");
}

int main(int argc, char *argv[]) {

    // Arguement check
    if (argc != 4) {
        char *arg_err = "Usage: ./a.out <newfile> <oldfile> <directory>\n";
        write(STDERR_FILENO, arg_err, strlen(arg_err));
        exit(1);
    }

    struct stat nfile_stat, ofile_stat, dir_stat;
    int err_check, dir_exist = 1;

    // Get nfile permissions
    err_check = stat(argv[3], &dir_stat);
    if (err_check == -1) {
        if (errno == ENOENT) {
            write_out("Directory is created", 0, "");
            dir_exist = 0;
        } else {
            perror("Error");
            exit(1);
        }
    } else {
        if (S_ISDIR(dir_stat.st_mode)) {
            write_out("Directory is created", 1, "");
        } else {
            char *error_message = "Argument 3 is not a directory\n\n";
            write(STDOUT_FILENO, error_message, strlen(error_message));
            write_out("Directory is created", 0, "");
        }
    }

    // Get ofile permissions
    err_check = stat(argv[2], &ofile_stat);
    if (err_check == -1) {
        perror("Error");
        exit(1);
    }

    err_check = stat(argv[1], &nfile_stat);
    if (err_check == -1) {
        perror("Error");
        exit(1);
    }

    // Check reversal
    check_reverse(argv[1], argv[2]);

    // Print directory permissions
    print_perm(nfile_stat.st_mode, "newfile");
    print_perm(ofile_stat.st_mode, "oldfile");
    if (dir_exist) {
        print_perm(dir_stat.st_mode, "directory");
    }
}

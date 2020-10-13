#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

    char *cmplt = "Percentage Completion: \n"; 

    // Write string to stdout
    ssize_t nwrote = write(STDOUT_FILENO, cmplt, strlen(cmplt));
    if (nwrote == -1) {
        perror("percentage");
        exit(1);
    }

    char per_out[10];

    // Print the percentage
    for (float curr_percent = 0; curr_percent < 100.01; curr_percent += 0.01) {
        sprintf(per_out, "\r%0.2f", curr_percent);

        // Write
        ssize_t nwrote = write(STDOUT_FILENO, per_out, strlen(per_out));
        if (nwrote == -1) {
            perror("percentage");
            exit(1);
        }
        sleep(0.01);
    }
    printf("\n");
}

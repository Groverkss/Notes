#include <stdio.h>
#include <stdlib.h>

const int BUFFER_SIZE = 512;

int main(int argc, char *argv[]) {
    for (int file_number = 1; file_number < argc; file_number++) {
        FILE *fp = fopen(argv[file_number], "r");
        if (fp == NULL) {
            printf("wcat: cannot open file\n");
            exit(1);
        }

        char buffer[BUFFER_SIZE];
        while(fgets(buffer, BUFFER_SIZE, fp)) {
            printf("%s", buffer);
        }
    }

    exit(0);
}    

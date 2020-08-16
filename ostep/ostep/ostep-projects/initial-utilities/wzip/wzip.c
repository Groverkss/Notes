#include <stdio.h>
#include <stdlib.h>

const int BUFFER_SIZE = 512;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }

    int nread = 0;
    char buffer[512];

    int counter = -1;
    char c;
    for (int file_num = 1; file_num < argc; file_num++) {
        FILE *fp = fopen(argv[file_num], "r");
        if (fp == NULL) {
            printf("wzip: cannot open file");
            exit(1);
        }

        while ((nread = fread(buffer, 1, BUFFER_SIZE, fp))) {
            for (int i = 0; i < nread; i++) {
                if (counter == -1) {
                    counter = 1;
                    c = buffer[i];
                } else if (c == buffer[i]) {
                    counter++;
                } else {
                    fwrite(&counter, 4, 1, stdout);
                    printf("%c", c);

                    counter = 1;
                    c = buffer[i];
                }
            }

        }
    }


    fwrite(&counter, 4, 1, stdout);
    printf("%c", c);
}

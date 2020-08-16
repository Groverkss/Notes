#include <stdio.h>
#include <stdlib.h>

const int BUFFER_SIZE = 100, DATA_SIZE = 5;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }
    
    int nread = 0;
    int buffer = 0;

    for (int file_number = 1; file_number < argc; file_number++) {
        FILE *fp = fopen(argv[file_number], "r");
        while((nread = fread(&buffer, 4, 1, fp))) {
            char c = getc(fp);
            for (int i = 0; i < buffer; i++) {
                printf("%c", c);
            }
        }
    }
}

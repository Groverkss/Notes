#include <stdio.h>
#include <stdlib.h>

const int BUFFER_SIZE = 512;

int main(int argc, char *argv[]) {

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("cannot open file\n");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, fp)) {
        printf("%s", buffer);
    }

    fclose(fp);
    return 0;
}

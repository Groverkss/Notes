#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_match(char *str, char *check, size_t str_size, size_t check_size) {
    for (int start = 0; start + check_size <= str_size; start++) {
        int flag = 1;
        for (int curr = 0; curr < check_size; curr++) {
            if (str[curr + start] != check[curr]) {
                flag = 0;
                break;
            }
        }

        if (flag) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }

    char *check = argv[1];
    char *line = NULL;

    size_t check_size = strlen(check);
    size_t str_size = 0;
    ssize_t nread = 0;

    if (argc == 2) {
        while ((nread = getline(&line, &str_size, stdin)) != -1) {
            if (check_match(line, check, nread, check_size)) {
                printf("%s", line);
            }
        }
    } else {
        for (int file_number = 2; file_number < argc; file_number++) {
            FILE *fp = fopen(argv[file_number], "r");
            if (fp == NULL) {
                printf("wgrep: cannot open file\n");
                exit(1);
            }

            while ((nread = getline(&line, &str_size, fp)) != -1) {
                if (check_match(line, check, nread, check_size)) {
                    printf("%s", line);
                }
            }
        }
    }
}

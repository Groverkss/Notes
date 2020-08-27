#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }
    size_t memory_use = atoi(argv[1]) * 1024l * 1024l;
    volatile char *volatile arr = malloc(memory_use);

    while(1) {
        for (int i = 0; i < memory_use; i++) {
            arr[i] = 0;
        }
    }
}

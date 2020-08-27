#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    ssize_t memory_use = atoi(argv[1]) * 1024 * 1024;
    volatile char *arr = malloc(memory_use);
    printf("%d\n", getpid());
    
    while(1) {
        for (int i = 0; i < memory_use; i++) {
            arr[i] = 0;
        }
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("hello world (pod:%d)\n", (int) getpid());
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        exit(1);
    } else {
        int rc_wait = wait(NULL);
        if (rc_wait == -1) {
            fprintf(stderr, "o no\n");
            exit(1);
        }
        printf("hello, I am parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait,
                (int) getpid());
    }
}

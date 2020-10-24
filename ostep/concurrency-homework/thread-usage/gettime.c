#include <stdio.h>
#include <sys/time.h>

struct timeval tv;

int main(void) {
    gettimeofday(&tv, NULL);

    printf("%ld\n", tv.tv_sec);
}

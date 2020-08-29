#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int *data = malloc(100);
    free(&data[50]);
}

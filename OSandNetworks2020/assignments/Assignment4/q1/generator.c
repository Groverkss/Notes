#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(void) {
    int n;
    scanf("%d", &n);

    int *arr = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % INT_MAX;
    }

    printf("%d\n", n);
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

const int SELECT_THRESHOLD = 5;

double get_time() {
    struct timeval t;
    struct timezone tzp;
    gettimeofday(&t, &tzp);
    return t.tv_sec + t.tv_usec*1e-6;
}

void merge(int arr[], int temp[], int start, int end) {
    // Copy contents of array to temperory array
    for (int i = start; i <= end; i++) {
        temp[i] = arr[i];
    }

    int mid = (start + end) >> 1;

    // Merge
    int curr_index = start, left_p = start, right_p = mid + 1;  
    while(left_p <= mid && right_p <= end) {
        if (temp[left_p] < temp[right_p]) {
            arr[curr_index] = temp[left_p];
            left_p++;
        } else {
            arr[curr_index] = temp[right_p];
            right_p++;
        }

        curr_index++;
    }

    // Put the remaining stuff back
    while(left_p <= mid) {
        arr[curr_index] = temp[left_p];
        curr_index++, left_p++;
    }

    while(right_p <= end) {
        arr[curr_index] = temp[right_p];
        curr_index++, right_p++;
    }
}

void selectsort(int arr[], int start, int end) {  
    int n = end - start + 1;
    for (int curr_index = 0; curr_index < n; curr_index++) {
        int min_i = -1;

        for (int j = start + curr_index; j <= end; j++) {
            if (min_i == -1 || arr[min_i] > arr[j]) {
                min_i = j; 
            }
        }

        int swap_tmp = arr[start + curr_index];
        arr[start + curr_index] = arr[min_i];
        arr[min_i] = swap_tmp;
    }
}

void mergesort(int arr[], int temp[], int start, int end) {
    if (end - start + 1 <= SELECT_THRESHOLD) {
        selectsort(arr, start, end);
    } else {
        int mid = (end + start) >> 1;
        mergesort(arr, temp, start, mid);
        mergesort(arr, temp, mid + 1, end);
        merge(arr, temp, start, end);
    }
}

void sort(int arr[], size_t n) {
    int *temp = malloc(sizeof(int) * n);
    mergesort(arr, temp, 0, n - 1);
}

int main(void) { 
    size_t n;
    scanf("%ld", &n);

    int *arr = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    double start_time = get_time(); 
    sort(arr, n);
    double end_time = get_time();

    // Enable for printing the array
    // for (int i = 0; i < n; i++) {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");

    fprintf(stderr, "%lf\n", end_time - start_time);
}

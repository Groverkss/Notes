#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/wait.h>

const int SELECT_THRESHOLD = 5;

double get_time() {
    struct timeval t;
    struct timezone tzp;
    gettimeofday(&t, &tzp);
    return t.tv_sec + t.tv_usec*1e-6;
}

int *sharemem(size_t size) {
    key_t mem_key = IPC_PRIVATE;
    int shm_id = shmget(mem_key, size, IPC_CREAT | 0666);
    return (int *)shmat(shm_id, NULL, 0);
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

void forksort(int arr[], int temp[], int start, int end) {
    if (end - start + 1 <= SELECT_THRESHOLD) {
        selectsort(arr, start, end);
    } else {
        int mid = (end + start) >> 1;

        pid_t child_1 = fork();

        if (child_1 == 0) {
            forksort(arr, temp, start, mid);
            exit(3);
        } else {
            pid_t child_2 = fork();

            if (child_2 == 0) {
                forksort(arr, temp, mid + 1, end);
                exit(3);
            }

            waitpid(child_1, NULL, 0);
            waitpid(child_2, NULL, 0);
        }

        merge(arr, temp, start, end);
    }
}

void sort(int arr[], size_t n) {
    int *temp = sharemem(sizeof(int) * n);
    forksort(arr, temp, 0, n - 1);
}

int main(void) { 
    size_t n;
    scanf("%ld", &n);

    int *arr = sharemem(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    double start_time = get_time(); 
    sort(arr, n);
    double end_time = get_time();

    // Enable for printing the array
    // for (int i = 0; i < n; i++) {
    //      printf("%d ", arr[i]);
    // }
    // printf("\n");

    fprintf(stderr, "%lf\n", end_time - start_time);
}

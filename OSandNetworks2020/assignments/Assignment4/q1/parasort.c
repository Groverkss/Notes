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
#include <pthread.h>

const int SELECT_THRESHOLD = 5;

typedef struct {
    int start;
    int end;
    int level;
    int *temp;
    int *arr;
} thread_data;

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
        for (int j = curr_index + start + 1; j <= end; j++) {
            int curr = j;
            while(curr != start && arr[curr] < arr[curr - 1]) {
                int temp_swap = arr[curr - 1];
                arr[curr - 1] = arr[curr];
                arr[curr] = temp_swap;
            }
        }
    }
}

void *thread_init(void *arg);

void parasort(int arr[], int temp[], int start, int end, int level) {
    if (end - start + 1 <= SELECT_THRESHOLD) {
        selectsort(arr, start, end);
    } else {
        int mid = (end + start) >> 1;

        if (level <= 2) {
            pthread_t child_left, child_right;

            thread_data *data_left = malloc(sizeof(thread_data));
            thread_data *data_right = malloc(sizeof(thread_data));

            data_left->start = start, data_left->end = mid;
            data_right->start = mid + 1, data_right->end = end;

            data_left->arr = data_right->arr = arr;
            data_left->temp = data_right->temp = temp;

            data_left->level = level + 1;
            data_right->level = level + 1;

            pthread_create(&child_left, NULL, thread_init, data_left);
            pthread_create(&child_right, NULL, thread_init, data_right);

            pthread_join(child_left, NULL);
            pthread_join(child_right, NULL);
        } else {
            parasort(arr, temp, start, mid, level + 1); 
            parasort(arr, temp, mid + 1, end, level + 1);
        } 

        merge(arr, temp, start, end);
    }
}

void *thread_init(void *arg) {
    thread_data *data = arg;
    parasort(data->arr, data->temp, data->start, data->end, data->level + 1);
}

void sort(int arr[], size_t n) {
    int *temp = sharemem(sizeof(int) * n);
    parasort(arr, temp, 0, n - 1, 0);
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
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");

    fprintf(stderr, "%lf\n", end_time - start_time);
}

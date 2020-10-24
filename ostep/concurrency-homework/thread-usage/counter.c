#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>

double get_time() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double) ((double)t.tv_sec + (double)t.tv_usec / 1e6);
}

volatile int counter;

pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;

void *worker(void *args) {
    while(true) {
        pthread_mutex_lock(&counter_lock);
        if (counter == 0) {
            pthread_mutex_unlock(&counter_lock);
            return NULL; 
        }
        counter--;
        pthread_mutex_unlock(&counter_lock);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("You wot m8\n");
        printf("Usage: ./a.out <threads> <counter>");
        return 1;
    } 

    int thread_num = atoi(argv[1]);
    counter = atoi(argv[2]);

    double start = get_time();
    pthread_t threads[thread_num];
    for (int i = 0; i < thread_num; i++) {
        pthread_create(&threads[i], NULL, worker, NULL);
    }

    for (int i = 0; i < thread_num; i++) {
        pthread_join(threads[i], NULL);
    }
    double end = get_time();

    printf("%f\n", end - start);
}

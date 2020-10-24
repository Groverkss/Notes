#include "libs.h"

int ALLOCATED_COUNT = 0;

void *Malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        perror("Memory Allocation Error");
        exit(1);
    }
    ALLOCATED_COUNT++;
    return ptr;
}

void Free(void *ptr) {
    free(ptr);
    ALLOCATED_COUNT--;
}

int Pthread_create(pthread_t *thread, const pthread_attr_t *attr,
        void *(*start_routine) (void *), void *arg) {
    int rc = pthread_create(thread, attr, start_routine, arg);
    if (rc != 0) {
        perror("Thread Create");
        exit(1);
    }
    return rc;
}

int Pthread_join(pthread_t thread, void **retval) {
    int rc = pthread_join(thread, retval);
    if (rc != 0) {
        perror("Thread Join");
        exit(1);
    }
    return rc;
}

void cprintf(const char *color, const char *format, ...) {
    printf("%s", color);

    va_list argptr;
    va_start(argptr, format);
    vprintf(format, argptr);
    va_end(argptr);

    printf("%s", RESET);
    fflush(stdout);
}

int get_random(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

void type(int index, char coord) {
    switch (coord) {
        case 'p':
            performers[index].stage = 2;
            break;
        case 'g':
            performers[index].stage = 2;
            break;
        case 'v':
            performers[index].stage = 0;
            break;
        case 'b':
            performers[index].stage = 1;
            break;
        case 's':
            performers[index].stage = 3;
            break;
    }
    return;
}

void Error(int index) {
    printf(
        "\033[1;31m\n"
        "%s leaves Srujana due to impatience\n",
        performers[index].name);

    fflush(stdout);
    performers[index].state = -1;
}

void Tshirt(int index) {
    if (coord == 0)
        return;
    sem_wait(&coordinators);

    printf(
        "\033[1;34m\n"
        "%s is collecting Tshirt\n",
        performers[index].name);
    fflush(stdout);

    sem_post(&coordinators);
}


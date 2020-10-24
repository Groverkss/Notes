#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <stdarg.h>
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

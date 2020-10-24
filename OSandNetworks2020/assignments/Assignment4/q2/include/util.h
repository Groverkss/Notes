#ifndef _UTIL_
#define _UTIL_

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define PINK "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"
#define RESET "\x1B[0m"

extern int ALLOCATED_COUNT;

void *Malloc(size_t size);

void Free(void *ptr);

int Pthread_create(pthread_t *thread, const pthread_attr_t *attr,
        void *(*start_routine) (void *), void *arg);

int Pthread_join(pthread_t thread, void **retval);

void cprintf(const char *color, const char *format, ...);

#endif

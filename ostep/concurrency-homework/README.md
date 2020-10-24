# Learning from Concurrency homeworks

## Thread API

- You can detect dataraces in codes using valgrind:

```C
valgrind --tool=helgrind ./executable
```

To get a smaller output with less information about syscalls:

```C
valgrind --tool=helgrind --history-level=none ./main-race
```

- Initialising a lock:

Static to default value:

```C
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
``` 

Dynamic way:

```C
pthread_mutex_t lock;
int rc = pthread_mutex_init(&lock, NULL);
assert(rc == 0) // Check success
```

- `helgrind` can can report possible deadlock errors by reporting non-ordered
acquisition of locks.

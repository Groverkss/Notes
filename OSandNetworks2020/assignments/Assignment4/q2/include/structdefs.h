#ifndef _STRUCTDEF_
#define _STRUCTDEF_

typedef struct {
    int index;
    float prob;
    int batches;
    int batch_capacity;
} Company;

typedef struct {
    int index; 
    float prob;
    int capacity;
    int phase;
    int slots;
    pthread_mutex_t lock;
    Queue *students;
} Zone;

typedef struct {
    int index; 
    int phase;
} Student;

#endif

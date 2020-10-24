#ifndef _QUEUE_
#define _QUEUE_

typedef struct {
    void **queue;
    int start;
    int end;
    size_t size;
} Queue;

Queue *queue_init(size_t size);

bool queue_empty(Queue *queue);

void queue_push(Queue *queue, void *entry);

void queue_pop(Queue *queue);

void *queue_front(Queue *queue);

#endif

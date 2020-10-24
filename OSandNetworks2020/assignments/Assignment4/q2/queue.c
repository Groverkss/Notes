#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include "libs.h"

Queue *queue_init(size_t size) {
    Queue *ret = Malloc(sizeof(Queue));
    ret->queue = Malloc(sizeof(void *) * (size + 1));
    ret->start = 0, ret->end = 0, ret->size = size;
    return ret;
}

bool queue_empty(Queue *queue) {
    if(queue->start == queue->end) {
        return true;
    } else {
        return false;
    }
}

void queue_push(Queue *queue, void *entry) {
    queue->queue[queue->end] = entry;
    queue->end++;
    if (queue->end == queue->size + 1) {
        queue->end = 0;
    } 
}

void queue_pop(Queue *queue) {
    queue->start++;
    if (queue->end == -1) {
        queue->end = queue->size;
    }
}

void *queue_front(Queue *queue) {
    void *ret = queue->queue[queue->start];
    return ret;
}

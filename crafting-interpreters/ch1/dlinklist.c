#include <stdlib.h>
#include "dlinklist.h"

struct LinkedList *createList(void) {
    struct LinkedList *init = malloc(sizeof(struct LinkedList *));
    init->size = 0;
    init->head = init->tail = NULL;
}

void insertElement(struct LinkedList *list, void *element) {
    struct Node *newElement = malloc(sizeof(struct Node *));

    if (list->tail != NULL) {
        /* Modify old tail */
        list->tail->next = newElement;

        /* Modify new tail */ 
        newElement->next = NULL;
        newElement->prev = list->tail;
        newElement->value = element;

        /* Change tail */
        list->tail = list->tail->next;
    } else {
        list->head = list->tail = newElement;    
        newElement->next = NULL;
        newElement->prev = NULL;
        newElement->value = element;
    }

    list->size++;
}

struct Node *findElement(struct LinkedList *list,
        void *element, int (*compare)(void *, void *)) {

    for (struct Node *curr = list->head; curr != NULL; curr = curr->next) {
        /* Use custom compare function */
        if (compare(curr->value, element)) {
            return curr;
        }
    }

    return NULL;
}

void deleteElement(struct Node *element) {
    if (element->prev != NULL) {
        element->prev->next = element->next;
    }

    if (element->next != NULL) {
        element->next->prev = element->prev;
    }
}

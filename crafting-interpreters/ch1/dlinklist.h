#ifndef _DLINKLIST_
#define _DLINKLIST_

struct Node {
    void *value;
    struct Node *next, *prev;
};

struct LinkedList {
    struct Node *head, *tail;
    int size;
};

struct LinkedList *createList(void);

void insertElement(struct LinkedList *list, void *element);

struct Node *findElement(struct LinkedList *list, void *element,
        int (*compare)(void *, void *));

void deleteElement(struct Node *element);

#endif

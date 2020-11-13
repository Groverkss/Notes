#include "dlinklist.h"
#include <stdio.h>
#include <stdlib.h>

int compare(void *el1, void *el2) {
    return *(int *)el1 == *(int *)el2;
}

void printlist(struct LinkedList *list) {
    for (struct Node *curr = list->head; curr != NULL; curr = curr->next) {
        printf("%d\n", *(int *)curr->value);
    } 
    printf("\n");
}

int main(void) {
    struct LinkedList *list = createList();

    int array[] = {1, 2, 3, 4, 5};

    int sizeArr = sizeof(array) / sizeof(int);

    for (int i = 0; i < sizeArr; i++) {
        insertElement(list, (void *)(array + i));
    } 

    printlist(list);

    int *testfind = malloc(sizeof(int));
    *testfind = 3;

    struct Node *todelete = findElement(list, testfind, compare);
    printf("%d\n\n", *(int *)todelete->value);

    deleteElement(todelete);
    printlist(list);
}

#include "ChainedLists.h"
#include <stdio.h>
#include <stdlib.h>

#define TYPELIST int
typedef struct chndLst {
    TYPELIST val;
    struct chndLst* next;

} list;


list* convertToList(TYPELIST* arr, int size) {
    if (size == 0) {
        return NULL;
    }

    list* res = (list *)malloc(1*sizeof(list));
    res->val = arr[0];
    res->next = convertToList(&arr[1], size - 1);
    
    return res;
}

void add(list* trgt, TYPELIST v) {
    if (trgt->next != NULL) {
        add(trgt->next, v);
    }

    else {
        list* newNode = (list *)malloc(1*sizeof(list));
        if (newNode == NULL) {
            printf("Memory allocation error !");
            exit(1);
        }

        newNode->val = v;
        trgt->next = newNode;
    }
}


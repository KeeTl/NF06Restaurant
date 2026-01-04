#include "ChainedLists.h"



int isEmpty(list_t* node) {return (node == NULL);}

list_t* convertToList(order_t* arr, int size) {
    if (size == 0) {
        return NULL;
    }

    list_t* res = (list_t *)malloc(1*sizeof(list_t));
    res->val = arr[0];
    res->next = convertToList(&arr[1], size - 1);
    
    return res;
}

list_t* createNode(order_t* v) {
    //printf("Creating node...");
    list_t* res = calloc(1, sizeof(list_t));
    if (res == NULL) {
        printf("Memory allocation error !\n");
        exit(1);
    }
    res->val = *v;
    res->next = NULL;
    return res;
}

void enQueue(list_t* trgt, order_t* v) {
    //printf("Enqueue...");
    if (!isEmpty(trgt->next)) {
        enQueue(trgt->next, v);
    }
    else {
        trgt->next = createNode(v);
    }
}

order_t deQueue(list_t* head) {
    order_t res = head->val;

    list_t* nxt = head->next;

    if (!isEmpty(nxt)) {
        head->val = nxt->val;
        head->next = nxt->next;

    }

    else {
        free(head);
        head = NULL;
    }
    return res;

}

void printList(list_t* head) {
    for (list_t* current = head; !isEmpty(current); current = current->next) {
        printOrder(&(current->val));
    }
}
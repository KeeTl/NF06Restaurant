#include "ChainedLists.h"



int isEmpty(list* node) {return (node == NULL);}

list* convertToList(order_t* arr, int size) {
    if (size == 0) {
        return NULL;
    }

    list* res = (list *)malloc(1*sizeof(list));
    res->val = arr[0];
    res->next = convertToList(&arr[1], size - 1);
    
    return res;
}

void enQueue(list* trgt, order_t v) {
    if (isEmpty(trgt->next)) {
        enQueue(trgt->next, v);
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

order_t deQueue(list* head) {
    order_t res = head->val;

    list* nxt = head->next;

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

#ifndef CHAINED_LISTS_H 
#define CHAINED_LISTS_H
    #include <stdio.h>
    #include <stdlib.h>
    #include "csvRestaurant.h"
    

    typedef struct chndLst {
        order_t val;
        struct chndLst* next;

    } list_t;

    int isEmpty(list_t* node);
    list_t* createNode(order_t* v);
    void enQueue(list_t* trgt, order_t* v);
    order_t deQueue(list_t* trgt);
    void printList(list_t* head);

#endif
#ifndef CHAINED_LISTS_H 
#define CHAINED_LISTS_H
    #include <stdio.h>
    #include <stdlib.h>
    #include "csvRestaurant.h"
    

    typedef struct chndLst {
        order_t val;
        struct chndLst* next;

    } list;

    int isEmpty(list* node);
    void enQueue(list* trgt, order_t v);
    order_t deQueue(list* trgt);

#endif
#ifndef CSV_RESTAURANT_H
#define CSV_RESTAURANT_H

    #include <stddef.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    typedef struct {
        enum PlateType {ENTREE,PLAT,DESSERT,BOISSON,OTHER} type;

        char* name;
        double price;
        long long stock;
    } menuItem_t;


    typedef struct {
        char* name;
        size_t count;
        menuItem_t* meals;
    } restaurant_t;

    typedef struct {
        restaurant_t* restaurant;
        unsigned long long ID;
        double total; 
    } order_t;

    void readRestaurantInfo(const char* filePath);

#endif
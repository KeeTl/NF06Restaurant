#ifndef CSV_RESTAURANT_H
#define CSV_RESTAURANT_H

    #include <stddef.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #define MAX_ITEMS_PER_RESTAURANT 15
    #define MAX_RESTAURANTS_NUMBER 10
    typedef struct {
        enum PlateType {ENTREE,PLAT,DESSERT,BOISSON,OTHER} type;

        char* name;
        double price;
        long long stock;
    } menuItem_t;


    typedef struct {
        char* name;
        size_t count;
        menuItem_t meals[MAX_ITEMS_PER_RESTAURANT];
    } restaurant_t;

    typedef struct {
        restaurant_t* restaurant;
        unsigned long long ID;
        double total; 
    } order_t;

    char** tokenSeparation(char* str, const char* separators, int maxNTokens, int* arrSze);
    restaurant_t* readRestaurantInfo(const char* filePath, size_t * arrSze);
    void printRestaurant(const restaurant_t* restaurant);
    void printOrder(const order_t* order);

#endif
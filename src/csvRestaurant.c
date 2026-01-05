#include "csvRestaurant.h"
#include <stdio.h>
#include <string.h>



char** tokenSeparation(char* str, const char* separators, int maxNTokens, int* arrSze) {
    char** tokens = (char **) calloc(maxNTokens, sizeof(char *));
    
    if (tokens == NULL) {
        exit(1);
    }
    *arrSze = 1;

    tokens[0] = strtok(str, separators);
    tokens[0][strcspn(tokens[0], "\n")] = '\0';

    for (char* token = strtok(NULL, separators);
    token != NULL && *arrSze <= maxNTokens;
    token = strtok(NULL, separators)) {

        tokens[*arrSze] = token;
        tokens[*arrSze][strcspn(tokens[*arrSze], "\n")] = '\0';
        (*arrSze)++;
    }

    tokens = realloc(tokens, (*arrSze)* sizeof(char*));
    

    return tokens;

}


menuItem_t readMealToken(char** tokens) {
    menuItem_t res;

    if (!strcmp(tokens[0], "Entree")) res.type = ENTREE;
    else if (!strcmp(tokens[0], "Plat")) res.type = PLAT;
    else if (!strcmp(tokens[0], "Dessert")) res.type = DESSERT;
    else if (!strcmp(tokens[0], "Boisson")) res.type = BOISSON;
    else res.type = OTHER;

    if (tokens[1][0] == ' ') {
    res.name = strdup(&tokens[1][1]); 
    }
    else {
        res.name = strdup(tokens[1]);
    }

    res.price = atof(tokens[2]);
    res.stock = atoll(tokens[3]);

    return res;
}


restaurant_t* readRestaurantInfo(const char* filePath, size_t * arrSze) {
    FILE* csv = fopen(filePath, "r");

    if (csv == NULL) {
        printf("File opening error !\n");   
        exit(1);
    }

    const size_t buffSize = 200;
    const size_t nMallocRestaurants = 10;
    size_t nRestaurants = 0;
    restaurant_t* res = (restaurant_t *)calloc(MAX_RESTAURANTS_NUMBER, sizeof(restaurant_t));

    char buffer[buffSize];
    char * separator = ",";    
        
    size_t nMeals = 0;

    while (fgets(buffer, buffSize, csv) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        int arrSze = 0;
        if (strlen(buffer)) {
            char** tkns = tokenSeparation(buffer, separator, 5, &arrSze);
            switch (arrSze) {
                case 1:
                    if (nRestaurants > 0) {
                        res[nRestaurants-1].count = nMeals;
                    }
                    res[nRestaurants ].name = strdup(tkns[0]);
                    nMeals = 0;
                    nRestaurants++;
                    break;
                case 4:
                    if (nMeals >= MAX_ITEMS_PER_RESTAURANT) exit(1);
                    if (nRestaurants == 0) exit(1);
                    res[nRestaurants - 1].meals[nMeals] = readMealToken(tkns);
                    nMeals++;
                    break;
                default:
                    printf("ERROR : file doesn't have a correct format !");
                    exit(2);
                    break;
            }
        }
    }
    fclose(csv);
    if (nRestaurants > 0) {
        res[nRestaurants-1].count = nMeals;
    }

    res = realloc(res, nRestaurants*sizeof(restaurant_t));
    *arrSze = nRestaurants;
    return res;
}

void printRestaurant(const restaurant_t* restaurant) {
    printf("%s\n", restaurant->name);
    for (int i = 0; i < restaurant->count; i++) {
        printf("%d.%20s", i+1, restaurant->meals[i].name);
        char* plateType;
        switch (restaurant->meals[i].type)
        {
        case ENTREE:
            plateType = "entree";
            break;
        case PLAT:
            plateType = "plat";
            break;
        case DESSERT:
            plateType = "dessert";
            break;
        case BOISSON:
            plateType = "boisson";
            break;
        default:
            plateType = "autre";
            break;
        }
        printf("%10s%6.2f%6lld\n", plateType,  restaurant->meals[i].price, restaurant->meals[i].stock);
    }
}

void printOrder(const order_t* order) {
    printf("%#5llx%20s%8.2f\n", order->ID, order->restaurant->name, order->total);
}
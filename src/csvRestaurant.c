#include "csvRestaurant.h"
#include <stdio.h>
#include <string.h>


char** tokenSeparation(char* str, const char* separators, int maxNTokens, int* arrSze) {
    char** tokens = (char **) calloc(maxNTokens, sizeof(char **));
    
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

    tokens = realloc(tokens, (*arrSze)* sizeof(char**));
    

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
        res.name = &tokens[1][1];
    }
    else {
        res.name = tokens[1];
    }

    res.price = atof(tokens[2]);
    res.stock = atoll(tokens[3]);

    return res;
}


void readRestaurantInfo(const char* filePath) {
    FILE* csv = fopen(filePath, "r");

    if (csv == NULL) {
        printf("File opening error !\n");
        exit(1);
    }

    const size_t buffSize = 200;
    const size_t nMallocRestaurants = 10;
    size_t nRestaurants = 0;
    restaurant_t* res = calloc(0, sizeof(restaurant_t));

    char buffer[buffSize];
    char * separator = ",";
    while (feof(csv) == 0) {
        fgets(buffer, buffSize, csv);
        int arrSze = 0;
        printf("%lu ", strlen(buffer));
        
        if (strcmp(buffer, "\0")) {
            char** tkns = tokenSeparation(buffer, separator, 5, &arrSze);
            switch (arrSze) {
                case 1:
                    

                    break;
                case 4:
                    

                default:
                    printf("ERROR : file doesn't have a correct format !");
                    exit(2);
            }
        }

        else {

        }
        

    }
    fclose(csv);
}


int main() {

    readRestaurantInfo("test.csv");

    return 0;
}
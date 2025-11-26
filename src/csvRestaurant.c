#include "csvRestaurant.h"
#include <stdio.h>
#include <stdlib.h>
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

void readRestaurantInfo(char* filePath) {
    FILE* csv = fopen(filePath, "r");

    if (csv == NULL) {
        printf("File opening error !\n");
        exit(1);
    }

    size_t buffSize = 200;
    char buffer[buffSize];
    char * separator = ",";
    while (feof(csv) == 0) {
        fgets(buffer, buffSize, csv);
        int arrSze = 0;
        char** tkns = tokenSeparation(buffer, separator, 5, &arrSze);
        printf("%d | ", arrSze);
        for (int i = 0; i < arrSze; i++) {
            if (!strcmp(tkns[i], "")) printf("\\0");
            printf("%s,",tkns[i]);
        }
        printf("\n");
        free(tkns);
    }
}

#include <string.h>
#include "hash.h"


hash_t strToHash(const char* str) {

    size_t strsze = strlen(str);
    hash_t hash = 5381;
    for (unsigned long i = 0; i < strsze; i++) {
        hash = ((hash << 5) - hash) + str[i]; 
        hash = hash & hash;
    }

    return hash;
}
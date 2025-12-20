#include "hash.h"

typedef enum usersEnum {
    CLIENT,
    ADMIN
} user_status_t;

typedef struct strctUser {
    char username[64];
    hash_t passwd;
    user_status_t status;
} user_t;
#include "users.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void addUserToDatabase(char* fpath, user_t* usr) {
    FILE* file = fopen(fpath, "a");
    if (file == NULL) {
        exit(1);
    }

    fwrite(usr, sizeof(user_t), 1, file);

    if (fclose(file) != 0) {
        exit(1);
    }

}


void displayUsers(char* fpath) {
    FILE* file = fopen(fpath, "rb");
    if (file == NULL) {
        printf("file opening err");
        exit(1);
    }

    user_t temp;

    while (fread(&temp, sizeof(user_t), 1, file) == 1) {
        printf("loop \n");
        printf(" %s %llu", temp.username, temp.passwd);
        if (temp.status == CLIENT) {
            printf(" CLIENT");
        }
        else if (temp.status == ADMIN) {
            printf(" ADMIN");
        }
        else {
            printf(" ??");
        }

        printf("\n");
    }

    if (fclose(file) != 0) {
        exit(1);
    }

}


int main() {
    
    user_t usr;
    strcpy(usr.username, "ligma"); 
    usr.passwd = strToHash("ballz");
    usr.status = CLIENT;

    addUserToDatabase("db.users", &usr);
    
    displayUsers("db.users");

    scanf("");

    return 0;
}


#ifndef USERS_H
#define USERS_H

    #include "hash.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    #define MAX_USERNAME_LENGTH 64

    typedef enum usersEnum {
        CLIENT,
        ADMIN
    } user_status_t;

    typedef struct strctUser {
        char username[MAX_USERNAME_LENGTH];
        hash_t passwd;
        user_status_t status;
    } user_t;

    typedef struct strctUserDatabase {

        user_t* users;
        size_t nUsers;
        
        char* fpath;
        unsigned int state;

    } user_database_t;

    typedef struct loginIStrct {
        int loginResult;
        user_t* user;
    } login_info_t;

    user_database_t newUserDatabase(const char* fpath);
    void scanUsers(user_database_t* db);
    void addUserToDatabase(user_database_t* db, const user_t* user);
    void modifyUserStatus(user_database_t* db, const char* username, user_status_t newStatus);
    login_info_t loginUser(const char* username, hash_t password, user_database_t* db);
    void freeUserDatabase(user_database_t* db);

#endif
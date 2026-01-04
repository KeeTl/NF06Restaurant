#include "users.h"

#define SIZE_BUFF_USER 10


int getUserId(const char* username, user_database_t* db) {
    for (int i = 0; i < db->nUsers; i++) {
        if (!strcmp(db->users[i].username, username)) return i;
    }
    return -1;
}

user_database_t newUserDatabase(const char* fpath) {
    user_database_t res;
    res.fpath = (char *)malloc(sizeof(char)*(strlen(fpath) + 1));
    strcpy(res.fpath, fpath);
    res.nUsers = 0;
    res.state = 0;
    res.users = NULL;
    return res;
}

void addUserToDatabase(user_database_t* db, const user_t* user) {
    if (db->state < 1) exit(1);
    if (loginUser(user->username, user->passwd, db).loginResult) exit(1);
    size_t newCount = db->nUsers + 1;
    user_t* tmp = realloc(db->users, newCount * sizeof(user_t));
    if (tmp == NULL) {
        printf("Memory allocation error !\n");
        exit(1);
    }
    db->users = tmp;

    db->users[newCount - 1] = *user;
    db->nUsers = newCount;

    FILE* file = fopen(db->fpath, "ab");
    if (file == NULL) {
        printf("ERROR ! File \"%s\" couldn't open properly!\n", db->fpath);
        exit(1);
    }

    if (fwrite(&db->users[newCount - 1], sizeof(user_t), 1, file) != 1) {
        printf("ERROR ! Writing has failed.\n");
        fclose(file);
        exit(1);
    }

    if (fclose(file) != 0) {
        exit(1);
    }
}   

void modifyUserStatus(user_database_t* db, const char* username, user_status_t newStatus) {
    if (db->state < 1) exit(1);
    int uID = getUserId(username, db);
    if (uID == -1) {
        printf("User not found !");
        return;
    }
    FILE* file = fopen(db->fpath, "rb+");
    if (file == NULL) {
        printf("ERROR ! File \"%s\" couldn't open properly!\n", db->fpath);
        exit(1);
    }

    if (fseek(file, sizeof(user_t)*uID, SEEK_SET) != 0) {
        fclose(file);
        printf("Error editing the file !");
        exit(1);
    }

    
    db->users[uID].status = newStatus;
    if (fwrite(&db->users[uID], sizeof(user_t), 1, file) != 1) {
        printf("ERROR ! Writing has failed.\n");
        fclose(file);
        exit(1);
    }

    fclose(file);
}

void scanUsers(user_database_t* db) {
    if (db->state != 0) {
        printf("ERROR ! Users are already loaded.");
        exit(1);
    }

    db->nUsers = 0;
    db->users = realloc(db->users, 0);

    FILE* file = fopen(db->fpath, "rb");
    if (file == NULL) {
        printf("ERROR ! File \"%s\" couldn't open properly!\n", db->fpath);
        exit(1);
    }

    user_t temp[SIZE_BUFF_USER];
    size_t nUsrs;
    while ((nUsrs = fread(temp, sizeof(user_t), SIZE_BUFF_USER, file)) > 0) {
        db->users = realloc(db->users, (db->nUsers + nUsrs)*sizeof(user_t));
        if (db->users == NULL) {
            printf("Memory allocation error !\n");
            exit(1);
        }
        
        for (size_t i = 0; i < nUsrs; i++) {
            db->users[db->nUsers + i] = temp[i];
        }
        db->nUsers += nUsrs;
    }

    db->state = 1;
    fclose(file);
}

login_info_t loginUser(const char* username, hash_t password, user_database_t* db) {
    login_info_t res;
    int uID = getUserId(username, db);
    if(uID == -1) {
        res.loginResult = 0;
        return res;
    }
    else {
        if (password == db->users[uID].passwd) {
                res.loginResult = 1;
                res.user = &(db->users[uID]);
                db->state = 2;
                return res;
            }
            res.loginResult = -1;
            return res;
    }
}

void freeUserDatabase(user_database_t* db) {
    free(db->users);
    db->users = NULL;
    db->state = 0;
    db->nUsers = 0;
}

#undef SIZE_BUFF_USER

/*
int main() {
    user_t usr;
    strcpy(usr.username, "admin");
    usr.passwd = strToHash("admin");
    usr.status = ADMIN;

    FILE* file = fopen("db.users", "wb");
    if (file == NULL) {
        exit(1);
    }

    fwrite(&usr, sizeof(user_t), 1, file);

    if (fclose(file) != 0) {
        exit(1);
    }

    return 0;
}
*/
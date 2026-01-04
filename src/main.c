#include <stdio.h>
#include "csvRestaurant.h"
#include "users.h"
#include "ChainedLists.h"

#define BUFFER_SIZE 200

int getRestaurantId(const char* rName, const restaurant_t* restaurants, size_t nRestaurants) {
    for (size_t i = 0; i < nRestaurants; i++) {
        if (!strcmp(restaurants[i].name, rName)) return i;
    }
    return -1;
}

int getPlateId(const char* pName, const restaurant_t* restaurant) {
    for (size_t i = 0; i < restaurant->count; i++) {
            if (!strcmp(restaurant->meals[i].name, pName)) return i;
    }
    return -1;
}

int main() { 
    char usercommand[BUFFER_SIZE];
    restaurant_t* restaurants = NULL;
    unsigned long long nOrders = 0;

    size_t nRestaurants;
    list_t* orders = NULL;

    printf("** == Restaurant Manager == **\n");
    user_database_t usersdb = newUserDatabase("db.users");
    scanUsers(&usersdb);

    #pragma region login
    login:
    
    char inputUsername[MAX_USERNAME_LENGTH];
    char inputPasswd[100];
    user_t* currentLogedUser = NULL;

    while (usersdb.state != 2 && usersdb.state == 1) {
        printf("username : ");
        fgets(inputUsername, MAX_USERNAME_LENGTH, stdin);
        inputUsername[strcspn(inputUsername, "\r\n")] = '\0';
        printf("password : ");
        fgets(inputPasswd, 100, stdin);
        inputPasswd[strcspn(inputPasswd, "\r\n")] = '\0';
        login_info_t loginInfo = loginUser(inputUsername, strToHash(inputPasswd), &usersdb);
        switch (loginInfo.loginResult) {
            case 0:
                printf("Username not found !\n");
                break;
            case -1:
                printf("Incorrect password !\n");
                break;
            case 1:
                currentLogedUser = loginInfo.user;
                printf("Welcome !\n\n");
                break;
            default:
                break;
        }
    }

    #pragma endregion

    while (usersdb.state == 2 && currentLogedUser != NULL) {
        printf(">>");
        fgets(usercommand, BUFFER_SIZE, stdin);
        int ntokens;        
        usercommand[strcspn(usercommand, "\r\n")] = '\0';
        char** tokens = tokenSeparation(usercommand, " ", 10, &ntokens);
        //for (int i = 0; i < ntokens; i++) printf("%s-", tokens[i]);
    
        if (!strcmp(tokens[0], "open")) {
            if (ntokens != 2) {
                printf("Invalid format ! Must be : \n open [filename]\n");
            }

            else {
                restaurants = readRestaurantInfo(tokens[1], &nRestaurants);
                printf("File opened. Type 'restaurants' to get the list\n");
            }
        }

        else if (!strcmp(tokens[0], "close")) {
            if (restaurants != NULL) {
                free(restaurants);
                restaurants = NULL;
            }
            else {
                printf("No file is opened !");
            }
        }

        else if ((currentLogedUser->status == ADMIN) && !strcmp(tokens[0], "user")){
            if (ntokens < 2) {
                printf("Missing argument ! ");
            }
            else if (!strcmp(tokens[1], "new")) {
                printf("Type user's username : ");
                fgets(inputUsername, MAX_USERNAME_LENGTH, stdin);
                inputUsername[strcspn(inputUsername, "\r\n")] = '\0';
                printf("Enter password : ");
                fgets(inputPasswd, 100, stdin);
                inputPasswd[strcspn(inputPasswd, "\r\n")] = '\0';
                hash_t passwd = strToHash(inputPasswd);

                printf("Confirm password : ");
                fgets(inputPasswd, 100, stdin);
                inputPasswd[strcspn(inputPasswd, "\r\n")] = '\0';
                hash_t passwdconfirmation = strToHash(inputPasswd);

                if (passwd != passwdconfirmation) {
                    printf("Passwords should be identical !");
                }             
                else {
                    user_t newUser;
                    strcpy(newUser.username, inputUsername);
                    newUser.passwd = passwd;
                    
                    char answ[3];

                    userStatus:
                    printf("Make this user admin (y : yes, n : no) ?");
                    fgets(answ, 3, stdin);
                    answ[strcspn(answ, "\n")] = '\0';
                    if (!strcmp(answ, "y")) {
                        newUser.status = ADMIN;
                    }
                    else if (!strcmp(answ, "n")) {
                        newUser.status = CLIENT;
                    }
                    else {
                        goto userStatus;
                    }

                    addUserToDatabase(&usersdb, &newUser);
                } 
            }
            else {
                printf("Unrecognised argument !\n");
            }
        }

        else if (!strcmp(tokens[0], "restaurants")) {
            if (restaurants == NULL) printf("No restaurants loaded ! \n");
            else {
                printf("%llu restaurant%s.\n\n", nRestaurants, (nRestaurants > 1) ? "s" : "");
                for (size_t i = 0; i < nRestaurants; i++) {
                    printRestaurant(&(restaurants[i]));
                    printf("\n");
                }   
            }
        }
        else if (!strcmp(tokens[0], "order")) {
            if (ntokens != 2) {
                printf("Invalid format ! Must be :\norder [restaurant name]\n");
            }
            else if (restaurants == NULL) {
                printf("No restaurants loaded !");
            }
            else {
                int n = getRestaurantId(tokens[1], restaurants, nRestaurants);
                if (n == -1) printf("Restaurant was not found");                
                else {
                    order_t newOrder;
                    newOrder.restaurant = &restaurants[n];
                    newOrder.total = 0;
                    
                    printRestaurant(&restaurants[n]);
                    int nPlates = 0;
                    order:
                    printf("Pick a plate (nothing = ending ordering) : ");
                    fgets(usercommand, MAX_ITEMS_PER_RESTAURANT, stdin);
                    usercommand[strcspn(usercommand, "\r\n")] = '\0';
                    int pId = getPlateId(usercommand, &restaurants[n]);

                    if (strcmp(usercommand, "")) {
                        if (pId == -1) {
                            printf("Plate isn't recognised !");
                        }

                        else {
                            printf("Pick a quantity : ");
                            fgets(usercommand, MAX_ITEMS_PER_RESTAURANT, stdin);                            
                            usercommand[strcspn(usercommand, "\r\n")] = '\0';
                            unsigned long qty = atol(usercommand);
                            
                            if ((restaurants[n].meals[pId].stock > 0 && restaurants[n].meals[pId].stock - qty >= 0) || (restaurants[n].meals[pId].stock < 0)) {
                                restaurants[n].meals[pId].stock  -= qty;
                                newOrder.total += restaurants[n].meals[pId].price * qty;
                                nPlates++;
                            }
                            else {
                                    printf("Invalid quantity ! max ammount :  %lu\n", restaurants[n].meals[pId].stock);
                                }
                            }
                            goto order;
                        }

                    if (nPlates != 0) {
                        newOrder.ID = ++nOrders;
                        printf("%llu", nOrders);
                        if (nOrders == 1) orders = createNode(&newOrder);
                        else enQueue(orders, &newOrder);
                        //printOrder(&newOrder);
                    }
                }
                
            }
        }

        else if ((currentLogedUser->status == ADMIN) && !strcmp(tokens[0], "queue")) {
            if (ntokens != 2) {
                printf("Missing argument !\n");
            }

            else {
                if (!strcmp(tokens[1], "display")) {
                    printList(orders);
                }

                else if (!strcmp(tokens[1], "next")) {
                    deQueue(orders);
                }

                else {
                    printf("Unrecognised argument!\n");
                }
            }
        }

        else if (!strcmp(tokens[0], "logout")) {
            usersdb.state = 1;
            currentLogedUser = NULL;
            goto login;
        }
        
        else {
            printf("Unknown command ! '%s'  is not recognised\n", tokens[0]);
        }

        free(tokens);
        usercommand[strcspn(usercommand, "\r\n")] = '\0';
        
        
    }
    freeUserDatabase(&usersdb);
    

    return 0;
}

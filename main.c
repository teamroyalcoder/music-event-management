/**
 * @file main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/utilities.h"
#include "include/splash.h"
#include "include/menu.h"
#include "include/login.h"

#define SHOWS_DATABASE "data/shows.txt"
#define SPLASH_DATABASE "data/splash.txt"
#define TICKETS_DATABASE "data/tickets.txt"
#define USERS_DATABASE "data/users.txt"

/**
 * Structure to hold user information.
 */
typedef struct {
    int id;
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
} User;

/**
 * @brief Struct representing a show
 */
typedef struct {
    int id;
    char singer[MAX_LENGTH];
    char date[MAX_LENGTH];
    char venue[MAX_LENGTH];
    char type[MAX_LENGTH];
    int price;
    int seats;
    char booked[MAX_LENGTH];
} Show;

/**
 * @brief Struct representing a ticket
 */
typedef struct {
    int id;
    char ticketNumber[MAX_LENGTH];
    int userId;
    int showId;
    int seatNumber;
    char paymentMethod[MAX_LENGTH];
    char paymentAccount[MAX_LENGTH];
    char transactionNumber[MAX_LENGTH];
    int status;
} Ticket;

int main() {
    splashScreen();
    int userid = login();
    if( userid >= 0 ) {
        menu( userid );
    }
    return 0;
}

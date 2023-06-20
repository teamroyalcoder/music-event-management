/**
 * @file src/login.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/login.h"
#include "../include/utilities.h"

#define MAX_USERS 2500
#define MAX_LENGTH 500

#define USERS_DATABASE "data/users.txt"

/**
 * Registers a new user.
 *
 * @param users Array of users.
 * @param numUsers Pointer to the number of existing users.
 *
 */
void registerUser( User users[], int *numUsers ) {
    if( *numUsers >= MAX_USERS ) {
        printf( "Maximum number of users reached!\n" );
        return;
    }
    User newUser;
    printf( "Enter username: " );
    fgets( newUser.username, sizeof( newUser.username ), stdin );
    newUser.username[strcspn( newUser.username, "\n" )] = '\0';
    for( int i = 0; i < *numUsers; i++ ) {
        if( strcmp( users[i].username, newUser.username ) == 0 ) {
            printf( "Username already exists! Please choose a different username.\n" );
            return;
        }
    }
    printf( "Enter password: " );
    disableEcho();
    fgets( newUser.password, sizeof( newUser.password ), stdin );
    enableEcho();
    newUser.password[strcspn( newUser.password, "\n" )] = '\0';
    newUser.id = *numUsers;
    users[*numUsers] = newUser;
    *numUsers += 1;
    printf( "\nRegistration successful!\n" );
    saveUsersToFile( users, *numUsers );
}

/**
 * Logs in a user.
 *
 * @param users Array of users.
 * @param numUsers Number of existing users.
 * @return User ID of the logged-in user, or -1 if login fails.
 */
int loginUser( User users[], int numUsers ) {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    printf( "Enter username: " );
    fgets( username, sizeof( username ), stdin );
    username[strcspn( username, "\n" )] = '\0';
    printf( "Enter password: " );
    disableEcho();
    fgets( password, sizeof( password ), stdin );
    enableEcho();
    password[strcspn( password, "\n" )] = '\0';
    for( int i = 0; i < numUsers; i++ ) {
        if( strcmp( users[i].username, username ) == 0 && strcmp( users[i].password, password ) == 0 ) {
            printf( "\nLogin successful!\n" );
            return i;
        }
    }
    printf( "Invalid username or password! Please try again.\n" );
    return loginUser( users, numUsers );
}

/**
 * Saves users to a text file.
 *
 * @param users Array of users.
 * @param numUsers Number of existing users.
 */
void saveUsersToFile( User users[], int numUsers ) {
    FILE *file = fopen( USERS_DATABASE, "w" );
    if( file == NULL ) {
        printf( "System error\n" );
        return;
    }
    fprintf( file, "id|username|password\n" );
    for( int i = 0; i < numUsers; i++ ) {
        fprintf( file, "%d|%s|%s\n", users[i].id, users[i].username, users[i].password );
    }
    fclose( file );
}

/**
 * Loads users from a text file.
 *
 * @param users Array of users.
 * @return Number of loaded users.
 */
int loadUsersFromFile( User users[] ) {
    FILE *file = fopen( USERS_DATABASE, "r" );
    if( file == NULL ) {
        printf( "System error\n" );
        return 0;
    }
    int numUsers = 0;
    char line[MAX_LENGTH * 3];
    fgets( line, sizeof( line ), file );
    while( fgets( line, sizeof( line ), file ) ) {
        sscanf( line, "%d|%[^|]|%s", &( users[numUsers].id ), users[numUsers].username, users[numUsers].password );
        numUsers++;
    }
    fclose( file );
    return numUsers;
}

/**
 * Handle login functionality
 *
 * @return User ID of the logged-in user, or -1 if login fails.
 */
int login() {
    User users[MAX_USERS];
    int numUsers = 0;
    numUsers = loadUsersFromFile( users );
    int option;
    int loggedInUserId = -1;
    do {
        printf( "\n--- Login or Register to continue ---\n" );
        printf( "\t1. Register\n" );
        printf( "\t2. Login\n" );
        printf( "\t3. Exit\n" );
        printf( "Enter an option: " );
        scanf( "%d", &option );
        getchar();
        switch( option ) {
        case 1:
            registerUser( users, &numUsers );
            loggedInUserId = numUsers - 1;
            break;
        case 2:
            loggedInUserId = loginUser( users, numUsers );
            break;
        case 3:
            printf( "Exiting...\n" );
            break;
        default:
            printf( "Invalid option! Please try again.\n" );
            break;
        }
    } while( option != 3 && loggedInUserId == -1 );
    if( loggedInUserId != -1 ) {
        printf( "Logged in as : %s\n", users[loggedInUserId].username );
    }
    saveUsersToFile( users, numUsers );
    return loggedInUserId;
}

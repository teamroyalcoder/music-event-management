/**
 * @file include/login.h
 */

#ifndef LOGIN_H
#define LOGIN_H

#define MAX_LENGTH 500

/**
 * Registers a new user.
 *
 * @param users Array of users.
 * @param numUsers Pointer to the number of existing users.
 *
 */
void registerUser( User users[], int *numUsers );

/**
 * Logs in a user.
 *
 * @param users Array of users.
 * @param numUsers Number of existing users.
 * @return User ID of the logged-in user, or -1 if login fails.
 */
int loginUser( User users[], int numUsers );

/**
 * Saves users to a text file.
 *
 * @param users Array of users.
 * @param numUsers Number of existing users.
 */
void saveUsersToFile( User users[], int numUsers );

/**
 * Loads users from a text file.
 *
 * @param users Array of users.
 * @return Number of loaded users.
 */
int loadUsersFromFile( User users[] );


/**
 * Handle login functionality
 *
 * @return User ID of the logged-in user, or -1 if login fails.
 */
int login();

#endif // LOGIN_H

/**
 * @file include/utilities.h
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdbool.h>

#define MAX_LENGTH 500

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

/**
 * @brief Function to disable terminal echo
 */
void disableEcho();

/**
 * @brief Function to enable terminal echo
 */
void enableEcho();
/**
 * @brief View upcoming shows and their available seats.
 *
 * This function reads the show details from the specified file and displays the upcoming shows along with their
 * available seats. It also allows selecting a show based on user input.
 *
 * @param filename The name of the file containing show details.
 * @param userId The user ID.
 * @param viewContent A flag indicating whether to display the show details.
 * @param hasSelect A flag indicating whether to allow show selection.
 * @param hasMenu A flag indicating whether to display a menu.
 * @return The selected show ID if `hasSelect` is true and a show is selected, otherwise -1.
 */
int viewUpcomingShows( const char* filename, int userId, bool viewContent, bool hasSelect, bool hasMenu );

/**
 * @brief Count the number of booked seats.
 *
 * This helper function counts the number of booked seats from the given string of booked seats.
 *
 * @param bookedSeats The string containing the booked seats, separated by commas.
 * @return The count of booked seats.
 */
int countBookedSeats( const char* bookedSeats );

/**
 * @brief Select and return number
 * @param serial Number of array
 * @return The ID of the selected show
 */
int selectPopup( int serial );

/**
 * @brief Buy tickets for the specified seat numbers.
 *
 * @param tickets tickets array of tickets
 * @param shows shows array of shows
 * @param ticketFilename The name of the tickets database file.
 * @param showFilename The name of the shows database file.
 * @param userId The ID of the user.
 * @param showId The ID of the show.
 */
void buyTicket( Ticket tickets[], Show shows[], const char* ticketFilename, const char* showFilename, int userId, int showId );

/**
 * @brief Displays show tickets based on a user ID and allows the user to select a ticket.
 *
 * @param tickets       Array of tickets.
 * @param ticketFilename The name of the tickets database file.
 * @param userId        ID of the user to filter the tickets.
 * @param viewContent        Whether it show content
 * @param hasSelect        Selection enabled?
 * @param hasMenu        Show menu?
 * @param forBooking        Show tickets for booking?
 *
 * @return The ID of the selected ticket, or -1 if no ticket is selected.
 */
int showTicketsByUserId( const Ticket tickets[], const char* ticketFilename, int userId, bool viewContent, bool hasSelect, bool hasMenu, bool forBooking );

/**
 * Update the booked field of a show by removing a seat number.
 *
 * @param booked The booked field string to update.
 * @param seatNumber The seat number to remove from the booked field.
 */
void updateBookedField( char* booked, int seatNumber );

/**
 * Update the booked field of a show in the shows.txt file.
 *
 * @param showId The ID of the show to update.
 * @param seatNumber The seat number to remove from the booked field.
 */
void updateBookedFieldInFile( int showId, int seatNumber );

/**
 * Updates the status of a ticket based on its ID.
 *
 * @param ticketId The ID of the ticket to update.
 * @param newStatus The new status for the ticket.
 */
void updateTicketStatus( int ticketId, int newStatus );

/**
 * Get the show ID and seat number from a ticket ID.
 *
 * @param ticketId The ID of the ticket.
 * @param showId Pointer to store the extracted show ID.
 * @param seatNumber Pointer to store the extracted seat number.
 */
void getShowIDAndSeatNumber( int ticketId, int* showId, int* seatNumber );

/**
 * @brief Generate a random code with the pattern of 3 letters, 5 numbers, and 1 letter.
 *
 * @param code The character array to hold the generated code.
 * @param codeLength The length of the code, including the null terminator.
 */
void generateRandomCode( char* code, int codeLength );

/**
 * @brief Generate a random alphanumeric character.
 *
 * @return The generated character.
 */
char generateRandomChar();

/**
 * @brief Generate a transaction number based on the current time.
 *
 * @param transactionNumber The array to store the generated transaction number.
 * @param size The size of the transactionNumber array.
 */
void generateTransactionNumber( char* transactionNumber, size_t size );

/**
 * Get the show date based on the show ID.
 *
 * @param shows Array of shows.
 * @param showId The ID of the show to retrieve the date for.
 * @param showDate Pointer to store the retrieved show date.
 * @return 1 if the show ID is found and the date is retrieved successfully, 0 otherwise.
 */
int getShowDateById( const Show shows[], int showId, char* showDate );

/**
 * @brief Converts a date string from the format "day,month,year" to "day month, year".
 *
 * This function takes an input date string in the format "day,month,year" and converts it
 * to the format "day month, year". The converted date is stored in the output buffer.
 *
 * @param[out] inputDate The input date string in the format "day,month,year".
 * @param[out] outputDate The output buffer to store the converted date.
 * @param[in] outputSize The size of the output buffer.
 */
void convertDate( const char* inputDate, char* outputDate, int outputSize );

#endif // UTILITIES_H

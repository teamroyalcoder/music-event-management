/**
 * @file src/utilities.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stddef.h>
#include "../include/utilities.h"

#define MAX_SHOW 100
#define MAX_TICKET 200
#define MAX_FIELD 200

#define SHOWS_DATABASE "data/shows.txt"
#define TICKETS_DATABASE "data/tickets.txt"

#ifdef _WIN32

#include <windows.h>

/**
 * @brief Function to disable terminal echo (Windows)
 */
void disableEcho() {
	HANDLE hStdin = GetStdHandle( STD_INPUT_HANDLE );
	DWORD mode;
	GetConsoleMode( hStdin, &mode );
	SetConsoleMode( hStdin, mode & ~ENABLE_ECHO_INPUT );
}

/**
 * @brief Function to enable terminal echo (Windows)
 */
void enableEcho() {
	HANDLE hStdin = GetStdHandle( STD_INPUT_HANDLE );
	DWORD mode;
	GetConsoleMode( hStdin, &mode );
	SetConsoleMode( hStdin, mode | ENABLE_ECHO_INPUT );
}

#else

#include <termios.h>

/**
 * @brief Function to disable terminal echo (Unix-like systems)
 */
void disableEcho() {
	struct termios term;
	tcgetattr( 0, &term );
	term.c_lflag &= ~ECHO;
	tcsetattr( 0, TCSANOW, &term );
}

/**
 * @brief Function to enable terminal echo (Unix-like systems)
 */
void enableEcho() {
	struct termios term;
	tcgetattr( 0, &term );
	term.c_lflag |= ECHO;
	tcsetattr( 0, TCSANOW, &term );
}

#endif

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
int viewUpcomingShows( const char* filename, int userId, bool viewContent, bool hasSelect, bool hasMenu ) {
	// Open the file for reading
	FILE* file = fopen( filename, "r" );
	if( file == NULL ) {
		return -1;
	}
	Show shows[100];
	char line[MAX_LENGTH];
	fgets( line, sizeof( line ), file );
	int numShows = 0;
	int serial = 1;
	time_t now = time( NULL );
	struct tm* timeinfo = localtime( &now );
	int currentYear = timeinfo->tm_year + 1900;
	int currentMonth = timeinfo->tm_mon + 1;
	int currentDay = timeinfo->tm_mday;
	int availableShowId[MAX_SHOW];
	while( fgets( line, sizeof( line ), file ) ) {
		sscanf( line, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%[^\n]", &shows[numShows].id, shows[numShows].singer,
				shows[numShows].date, shows[numShows].venue, shows[numShows].type, &shows[numShows].price,
				&shows[numShows].seats, shows[numShows].booked );
		int showYear, showMonth, showDay;
		sscanf( shows[numShows].date, "%d,%d,%d", &showDay, &showMonth, &showYear );
		if( viewContent ) {
			if( showYear > currentYear || ( showYear == currentYear && showMonth > currentMonth ) ||
					( showYear == currentYear && showMonth == currentMonth && showDay >= currentDay ) ) {
				char formattedDate[30];
				convertDate( shows[numShows].date, formattedDate, sizeof( formattedDate ) );
				availableShowId[serial - 1] = shows[numShows].id;
				int bookedSeats = countBookedSeats( shows[numShows].booked );
				int availableSeats = shows[numShows].seats - bookedSeats;
				printf( "\t[0]Show: %d\n", serial );
				printf( "\t[0]Singer: %s\n", shows[numShows].singer );
				printf( "\t[0]Date: %s\n", formattedDate );
				printf( "\t[0]Venue: %s\n", shows[numShows].venue );
				printf( "\t[0]Type: %s\n", shows[numShows].type );
				printf( "\t[0]Available Seats: %d\n", availableSeats );
				printf( "\n\n" );
				serial++;
			}
		}
		numShows++;
	}
	fclose( file );
	if( serial - 1 == 0 ) {
		printf( "No shows found!\n" );
		return -1;
	}
	if( hasSelect ) {
		printf( "Select a show " );
		int selectedCustomSerial = selectPopup( serial );
		return availableShowId[selectedCustomSerial - 1];
	}
	return -1;
}

/**
 * @brief Count the number of booked seats.
 *
 * This helper function counts the number of booked seats from the given string of booked seats.
 *
 * @param bookedSeats The string containing the booked seats, separated by commas.
 * @return The count of booked seats.
 */
int countBookedSeats( const char* bookedSeats ) {
	int count = 0;
	char* token = strtok( bookedSeats, "," );
	while( token != NULL ) {
		count++;
		token = strtok( NULL, "," );
	}
	return count;
}

/**
 * @brief Select and return number
 * @param serial Number of array
 * @return The ID of the selected show
 */
int selectPopup( int serial ) {
	int selectedCustomSerial = -1;
	int validInput = 0;
	while( !validInput ) {
		printf( "(-1 to cancel): " );
		scanf( "%d", &selectedCustomSerial );
		if( selectedCustomSerial >= 1 && selectedCustomSerial <= serial ) {
			validInput = 1;
		} else if( selectedCustomSerial == -1 ) {
			printf( "Canceled.\n" );
			validInput = 1;
			return -1;
		} else {
			printf( "Invalid selection. Please try again.\n" );
		}
	}
	return selectedCustomSerial;
}

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
void buyTicket( Ticket tickets[], Show shows[], const char* ticketFilename, const char* showFilename, int userId, int showId ) {
	srand( time( NULL ) );
	FILE *fileTicket = fopen( ticketFilename, "r" );
	FILE *fileShow = fopen( showFilename, "r" );
	if( fileTicket == NULL ) {
		printf( "System error, please contact with respective developers..\n" );
		return;
	}
	if( fileShow == NULL ) {
		printf( "System error, please contact with respective developers..\n" );
		fclose( fileShow );
		return;
	}
	int numTickets = 0;
	int numShows = 0;
	char line[MAX_LENGTH * 3];
	fgets( line, sizeof( line ), fileTicket );
	fgets( line, sizeof( line ), fileShow );
	while( fgets( line, sizeof( line ), fileTicket ) ) {
		sscanf( line, "%d|%[^|]|%d|%d|%d|%[^|]|%[^|]|%[^|]|%d", &( tickets[numTickets].id ),
				tickets[numTickets].ticketNumber,
				&( tickets[numTickets].userId ),
				&( tickets[numTickets].showId ),
				&( tickets[numTickets].seatNumber ),
				tickets[numTickets].paymentMethod,
				tickets[numTickets].paymentAccount,
				tickets[numTickets].transactionNumber,
				&( tickets[numTickets].status ) );
		numTickets++;
	}
	while( fgets( line, sizeof( line ), fileShow ) ) {
		sscanf( line, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%[^\n]", &( shows[numShows].id ),
				shows[numShows].singer,
				shows[numShows].date,
				shows[numShows].venue,
				shows[numShows].type,
				&( shows[numShows].price ),
				&( shows[numShows].seats ),
				shows[numShows].booked );
		numShows++;
	}
	fclose( fileTicket );
	fclose( fileShow );
	for( int i = 0; i < numShows; ++i ) {
		if( shows[i].id == showId ) {
			printf( "Cost for %s's %s show is %d BDT/ticket\n", shows[i].singer, shows[i].type, shows[i].price );
			int seat_quantity;
			printf( "How many seats do you want to buy? (1 seat/ticket): " );
			scanf( "%d", &seat_quantity );
			int seat_numbers[seat_quantity];
			printf( "Available seats: " );
			int j;
			int isFirst = 1;
			for( j = 1; j <= shows[i].seats; j++ ) {
				#if defined(_WIN32) || defined(_WIN64)
					_itoa_s(j, line, 10);
				#elif defined(__APPLE__) || defined(__MACH__)
					snprintf(line, sizeof(line), "%d", j);
				#else
					sprintf(line, "%d", j);
				#endif
				if( strstr( shows[i].booked, line ) == NULL ) {
					if( isFirst ) {
						printf( "%d", j );
						isFirst = 0;
					} else {
						printf( ", %d", j );
					}
				}
			}
			char ticketNumbers[seat_quantity][10];
			printf( "\nSelect seat(s) from above available seat(s): " );
			for( int k = 0; k < seat_quantity; ++k ) {
				int seat_number;
				scanf( "%d", &seat_number );
				for( int m = 0; m < k; ++m ) {
					if( seat_number == seat_numbers[m] ) {
						printf( "Duplicate seat number detected! Please select unique seats.\n" );
						return;
					}
				}
				#if defined(_WIN32) || defined(_WIN64)
					_itoa_s(seat_number, line, 10);
				#elif defined(__APPLE__) || defined(__MACH__)
					snprintf(line, sizeof(line), "%d", seat_number);
				#else
					sprintf(line, "%d", seat_number);
				#endif
				if( strstr( shows[i].booked, line ) != NULL ) {
					printf( "Seat number %d is already booked!\n", seat_number );
					return;
				}
				seat_numbers[k] = seat_number;
				generateRandomCode( ticketNumbers[k], 10 );
			}
			printf( "You have selected %d (", seat_quantity );
			for( int k = 0; k < seat_quantity; ++k ) {
				printf( "%d", seat_numbers[k] );
				if( k + 1 != seat_quantity ) {
					printf( ", " );
				}
			}
			printf( ") totaling %d BDT\n", shows[i].price * seat_quantity );
			const char *payment_method;
			printf( "Please select a payment method\n" );
			printf( "\t1.bKash\n" );
			printf( "\t2.Nagad\n" );
			printf( "\t3.Rocket\n" );
			printf( "Select: " );
			int payment_m;
			scanf( "%d", &payment_m );
			if( payment_m >= 1 && payment_m <= 3 ) {
				if( payment_m == 1 ) {
					payment_method = "bKash";
				} else if( payment_m == 2 ) {
					payment_method = "Nagad";
				} else {
					payment_method = "Rocket";
				}
			} else {
				return;
			}
			scanf( "%*[^\n]" );
			scanf( "%*c" );
			printf( "Please enter your %s account number: ", payment_method );
			char payment_account[MAX_FIELD];
			fgets( payment_account, sizeof( payment_account ), stdin );
			payment_account[strcspn( payment_account, "\n" )] = '\0';
			char transactionNum[10];
			generateTransactionNumber( transactionNum, sizeof( transactionNum ) );
			printf( "\nThank you! Transaction ID %s, %d ticket(s) purchased, and %d BDT credited from your %s account (%s).\n", transactionNum, seat_quantity, shows[i].price * seat_quantity, payment_method, payment_account );
			printf( "Purchased ticket(s):\n" );
			for( int k = 0; k < seat_quantity; ++k ) {
				printf( "\t%s\n", ticketNumbers[k] );
				tickets[numTickets].id = numTickets;
				strcpy( tickets[numTickets].ticketNumber, ticketNumbers[k] );
				tickets[numTickets].userId = userId;
				tickets[numTickets].showId = showId;
				tickets[numTickets].seatNumber = seat_numbers[k];
				strcpy( tickets[numTickets].paymentMethod, payment_method );
				strcpy( tickets[numTickets].paymentAccount, payment_account );
				strcpy( tickets[numTickets].transactionNumber, transactionNum );
				tickets[numTickets].status = 1;
				numTickets++;
			}
			char updatedBooked[MAX_LENGTH] = "";
			strcpy( updatedBooked, shows[i].booked );
			for( int j = 0; j < seat_quantity; j++ ) {
				int selectedSeat = seat_numbers[j];
				#if defined(_WIN32) || defined(_WIN64)
					_itoa_s(selectedSeat, line, 10);
				#elif defined(__APPLE__) || defined(__MACH__)
					snprintf(line, sizeof(line), "%d", selectedSeat);
				#else
					sprintf(line, "%d", selectedSeat);
				#endif
				if( strstr( shows[i].booked, line ) == NULL ) {
					if( strlen( shows[i].booked ) > 0 || j > 0 ) {
						strcat( updatedBooked, "," );
					}
					sprintf( line, "%d", selectedSeat );
					strcat( updatedBooked, line );
				}
			}
			strcpy( shows[i].booked, updatedBooked );
			break;
		}
	}
	fileTicket = fopen( ticketFilename, "w" );
	fileShow = fopen( showFilename, "w" );
	if( fileTicket == NULL ) {
		printf( "System error, please contact with respective developers.\n" );
		return;
	}
	if( fileShow == NULL ) {
		printf( "System error, please contact with respective developers.\n" );
		fclose( fileShow );
		return;
	}
	fprintf( fileTicket, "id|ticket_number|user_id|show_id|seat_number|payment_method|payment_account|transaction_number|status\n" );
	fprintf( fileShow, "id|singer|date|type|price|seats|booked\n" );
	for( int i = 0; i < numTickets; i++ ) {
		fprintf( fileTicket, "%d|%s|%d|%d|%d|%s|%s|%s|%d\n", tickets[i].id,
				 tickets[i].ticketNumber,
				 tickets[i].userId,
				 tickets[i].showId,
				 tickets[i].seatNumber,
				 tickets[i].paymentMethod,
				 tickets[i].paymentAccount,
				 tickets[i].transactionNumber,
				 tickets[i].status );
	}
	for( int i = 0; i < numShows; i++ ) {
		fprintf( fileShow, "%d|%s|%s|%s|%s|%d|%d|%s\n",
				 shows[i].id,
				 shows[i].singer,
				 shows[i].date,
				 shows[i].venue,
				 shows[i].type,
				 shows[i].price,
				 shows[i].seats,
				 shows[i].booked );
	}
	fclose( fileTicket );
	fclose( fileShow );
}

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
int showTicketsByUserId( const Ticket tickets[], const char* ticketFilename, int userId, bool viewContent, bool hasSelect, bool hasMenu, bool forBooking ) {
	FILE* file = fopen( SHOWS_DATABASE, "r" );
	if( file == NULL ) {
		return -1;
	}
	Show shows[100];
	char line[MAX_LENGTH];
	fgets( line, sizeof( line ), file );
	int numShows = 0;
	while( fgets( line, sizeof( line ), file ) ) {
		sscanf( line, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%[^\n]", &shows[numShows].id, shows[numShows].singer,
				shows[numShows].date, shows[numShows].venue, shows[numShows].type, &shows[numShows].price,
				&shows[numShows].seats, shows[numShows].booked );
		numShows++;
	}
	FILE *fileTicket = fopen( ticketFilename, "r" );
	if( fileTicket == NULL ) {
		printf( "System error, please contact with respective developers..\n" );
	}
	int numTickets = 0;
	fgets( line, sizeof( line ), fileTicket );
	while( fgets( line, sizeof( line ), fileTicket ) ) {
		sscanf( line, "%d|%[^|]|%d|%d|%d|%[^|]|%[^|]|%[^|]|%d", &( tickets[numTickets].id ),
				tickets[numTickets].ticketNumber,
				&( tickets[numTickets].userId ),
				&( tickets[numTickets].showId ),
				&( tickets[numTickets].seatNumber ),
				tickets[numTickets].paymentMethod,
				tickets[numTickets].paymentAccount,
				tickets[numTickets].transactionNumber,
				&( tickets[numTickets].status ) );
		numTickets++;
	}
	fclose( fileTicket );
	int serial = 1;
	int availableTickets[MAX_TICKET];
	if( !forBooking ) {
		for( int i = 0; i < numTickets; ++i ) {
			if( tickets[i].userId == userId ) {
				availableTickets[serial - 1] = tickets[i].id;
				if( viewContent ) {
					printf( "\t[0]Ticket: %d\n", serial );
					printf( "\t[0]Ticket Number: %s\n", tickets[i].ticketNumber );
					for( int x = 0; x < numShows; ++x ) {
						if( tickets[i].showId == shows[x].id ) {
							printf( "\t[0]Show: %s's %s show\n", shows[x].singer, shows[x].type );
							printf( "\t[0]Venue: %s\n", shows[x].venue );
						}
					}
					printf( "\t[0]Seat Number: %d\n", tickets[i].seatNumber );
					printf( "\t[0]Payment Method: %s\n", tickets[i].paymentMethod );
					printf( "\t[0]Payment Account: %s\n", tickets[i].paymentAccount );
					printf( "\t[0]Transaction Number: %s\n", tickets[i].transactionNumber );
					if( tickets[i].status ) {
						for( int x = 0; x < numShows; ++x ) {
							if( tickets[i].showId == shows[x].id ) {
								time_t now = time( NULL );
								struct tm* timeinfo = localtime( &now );
								int currentYear = timeinfo->tm_year + 1900;
								int currentMonth = timeinfo->tm_mon + 1;
								int currentDay = timeinfo->tm_mday;
								int showYear, showMonth, showDay;
								sscanf( shows[x].date, "%d,%d,%d", &showDay, &showMonth, &showYear );
								if( showYear > currentYear || ( showYear == currentYear && showMonth > currentMonth ) ||
										( showYear == currentYear && showMonth == currentMonth && showDay >= currentDay ) ) {
									printf( "\t[0]Status: Active\n" );
								} else {
									printf( "\t[0]Status: Expired\n" );
								}
							}
						}
					} else {
						printf( "\t[0]Status: Canceled\n" );
					}
					printf( "\n\n" );
				}
				++serial;
			}
		}
	} else {
		for( int i = 0; i < numTickets; ++i ) {
			for( int x = 0; x < numShows; ++x ) {
				if( tickets[i].showId == shows[x].id ) {
					time_t now = time( NULL );
					struct tm* timeinfo = localtime( &now );
					int currentYear = timeinfo->tm_year + 1900;
					int currentMonth = timeinfo->tm_mon + 1;
					int currentDay = timeinfo->tm_mday;
					int showYear, showMonth, showDay;
					sscanf( shows[x].date, "%d,%d,%d", &showDay, &showMonth, &showYear );
					if( tickets[i].userId == userId && ( showYear > currentYear || ( showYear == currentYear && showMonth > currentMonth ) ||
														 ( showYear == currentYear && showMonth == currentMonth && showDay >= currentDay ) ) ) {
						availableTickets[serial - 1] = tickets[i].id;
						if( viewContent ) {
							printf( "\t[0]Ticket: %d\n", serial );
							printf( "\t[0]Ticket Number: %s\n", tickets[i].ticketNumber );
							for( int x = 0; x < numShows; ++x ) {
								if( tickets[i].showId == shows[x].id ) {
									printf( "\t[0]Show: %s's %s show\n", shows[x].singer, shows[x].type );
									printf( "\t[0]Venue: %s\n", shows[x].venue );
								}
							}
							printf( "\t[0]Seat Number: %d\n", tickets[i].seatNumber );
							printf( "\t[0]Payment Method: %s\n", tickets[i].paymentMethod );
							printf( "\t[0]Payment Account: %s\n", tickets[i].paymentAccount );
							printf( "\t[0]Transaction Number: %s\n", tickets[i].transactionNumber );
							if( tickets[i].status ) {
								for( int x = 0; x < numShows; ++x ) {
									if( tickets[i].showId == shows[x].id ) {
										time_t now = time( NULL );
										struct tm* timeinfo = localtime( &now );
										int currentYear = timeinfo->tm_year + 1900;
										int currentMonth = timeinfo->tm_mon + 1;
										int currentDay = timeinfo->tm_mday;
										int showYear, showMonth, showDay;
										sscanf( shows[x].date, "%d,%d,%d", &showDay, &showMonth, &showYear );
										if( showYear > currentYear || ( showYear == currentYear && showMonth > currentMonth ) ||
												( showYear == currentYear && showMonth == currentMonth && showDay >= currentDay ) ) {
											printf( "\t[0]Status: Active\n" );
										} else {
											printf( "\t[0]Status: Expired\n" );
										}
									}
								}
							} else {
								printf( "\t[0]Status: Canceled\n" );
							}
							printf( "\n\n" );
						}
						++serial;
					}
				}
			}
		}
	}
	if( serial - 1 == 0 ) {
		printf( "No tickets found!\n" );
		return -1;
	}
	if( hasSelect ) {
		printf( "Select a ticket " );
		int selectedTicketId = selectPopup( serial );
		return availableTickets[selectedTicketId - 1];
	}
	return -1;
}

/**
 * Update the booked field of a show by removing a seat number.
 *
 * @param booked The booked field string to update.
 * @param seatNumber The seat number to remove from the booked field.
 */
void updateBookedField( char* booked, int seatNumber ) {
	int length = strlen( booked );
	int i, j;
	int commaCount = 0;
	for( i = 0; i < length; i++ ) {
		if( booked[i] == ',' ) {
			commaCount++;
		}
	}
	char updatedBooked[length + 1];
	memset( updatedBooked, '\0', sizeof( updatedBooked ) );
	j = 0;
	for( i = 0; i < length; i++ ) {
		if( booked[i] != ',' ) {
			int currentSeat = 0;
			while( i < length && booked[i] != ',' ) {
				currentSeat = ( currentSeat * 10 ) + ( booked[i] - '0' );
				i++;
			}
			if( currentSeat != seatNumber ) {
				sprintf( &updatedBooked[j], "%d", currentSeat );
				j += strlen( &updatedBooked[j] );
				if( i < length ) {
					updatedBooked[j] = ',';
					j++;
				}
			}
		}
	}
	strcpy( booked, updatedBooked );
}

/**
 * Update the booked field of a show in the shows.txt file.
 *
 * @param showId The ID of the show to update.
 * @param seatNumber The seat number to remove from the booked field.
 */
void updateBookedFieldInFile( int showId, int seatNumber ) {
	Show shows[MAX_SHOW];
	int numShows = 0;
	FILE* file = fopen( SHOWS_DATABASE, "r" );
	if( file == NULL ) {
		printf( "System error\n" );
		return;
	}
	char line[200];
	fgets( line, sizeof( line ), file );
	while( fgets( line, sizeof( line ), file ) != NULL && numShows < MAX_SHOW ) {
		sscanf( line, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%[^\n]",
				&( shows[numShows].id ), shows[numShows].singer, shows[numShows].date, shows[numShows].venue,
				shows[numShows].type, &( shows[numShows].price ), &( shows[numShows].seats ),
				shows[numShows].booked );
		numShows++;
	}
	fclose( file );
	if( showId >= 0 && showId < numShows ) {
		updateBookedField( shows[showId].booked, seatNumber );
		file = fopen( SHOWS_DATABASE, "w" );
		if( file == NULL ) {
			printf( "Error opening file for writing: %s\n", SHOWS_DATABASE );
			return;
		}
		fprintf( file, "id|singer|date|venue|type|price|seats|booked\n" );
		for( int i = 0; i < numShows; i++ ) {
			fprintf( file, "%d|%s|%s|%s|%s|%d|%d|%s\n",
					 shows[i].id, shows[i].singer, shows[i].date, shows[i].venue,
					 shows[i].type, shows[i].price, shows[i].seats,
					 shows[i].booked );
		}
		fclose( file );
		printf( "Ticket updated successfully\n" );
	} else {
		printf( "Invalid show ID\n" );
	}
}

/**
 * Get the show ID and seat number from a ticket ID.
 *
 * @param ticketId The ID of the ticket.
 * @param showId Pointer to store the extracted show ID.
 * @param seatNumber Pointer to store the extracted seat number.
 */
void getShowIDAndSeatNumber( int ticketId, int* showId, int* seatNumber ) {
	FILE* file = fopen( TICKETS_DATABASE, "r" );
	if( file == NULL ) {
		printf( "System error\n" );
		return;
	}
	char line[1000];
	while( fgets( line, sizeof( line ), file ) ) {
		int currentTicketId, currentShowId, currentSeatNumber;
		sscanf( line, "%d|%*[^|]|%*[^|]|%d|%d|%*[^|]|%*[^|]|%*[^|]|%*[^|]", &currentTicketId, &currentShowId, &currentSeatNumber );
		if( currentTicketId == ticketId ) {
			*showId = currentShowId;
			*seatNumber = currentSeatNumber;
			break;
		}
	}
	fclose( file );
}

/**
 * Updates the status of a ticket based on its ID.
 *
 * @param ticketId The ID of the ticket to update.
 * @param newStatus The new status for the ticket.
 */
void updateTicketStatus( int ticketId, int newStatus ) {
	Ticket tickets[MAX_TICKET];
	int numTickets = 0;
	FILE* file = fopen( TICKETS_DATABASE, "r" );
	if( file == NULL ) {
		printf( "System error\n" );
		return;
	}
	char line[200];
	fgets( line, sizeof( line ), file );
	while( fgets( line, sizeof( line ), file ) != NULL && numTickets < MAX_TICKET ) {
		sscanf( line, "%d|%[^|]|%d|%d|%d|%[^|]|%[^|]|%[^|]|%d",
				&( tickets[numTickets].id ), tickets[numTickets].ticketNumber, &( tickets[numTickets].userId ),
				&( tickets[numTickets].showId ), &( tickets[numTickets].seatNumber ),
				tickets[numTickets].paymentMethod, tickets[numTickets].paymentAccount,
				tickets[numTickets].transactionNumber, &( tickets[numTickets].status ) );
		numTickets++;
	}
	fclose( file );
	for( int i = 0; i < numTickets; i++ ) {
		if( tickets[i].id == ticketId ) {
			if( tickets[i].status == 0 ) {
				printf( "Ticket is already canceled\n" );
				break;
			}
			tickets[i].status = newStatus;
			updateBookedFieldInFile( tickets[i].showId, tickets[i].seatNumber );
		}
	}
	file = fopen( TICKETS_DATABASE, "w" );
	if( file == NULL ) {
		return;
	}
	fprintf( file, "id|ticket_number|user_id|show_id|seat_number|payment_method|payment_account|transaction_number|status\n" );
	for( int i = 0; i < numTickets; i++ ) {
		fprintf( file, "%d|%s|%d|%d|%d|%s|%s|%s|%d\n",
				 tickets[i].id, tickets[i].ticketNumber, tickets[i].userId,
				 tickets[i].showId, tickets[i].seatNumber,
				 tickets[i].paymentMethod, tickets[i].paymentAccount,
				 tickets[i].transactionNumber, tickets[i].status );
	}
	fclose( file );
}

/**
 * @brief Generate a random code with the pattern of 3 letters, 5 numbers, and 1 letter.
 *
 * @param code The character array to hold the generated code.
 * @param codeLength The length of the code, including the null terminator.
 */
void generateRandomCode( char* code, int codeLength ) {
	const char* letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char* numbers = "0123456789";
	for( int i = 0; i < 3; i++ ) {
		code[i] = letters[rand() % 26];
	}
	for( int i = 3; i < 8; i++ ) {
		code[i] = numbers[rand() % 10];
	}
	code[8] = letters[rand() % 26];
	code[codeLength - 1] = '\0';
}

/**
 * @brief Generate a random alphanumeric character.
 *
 * @return The generated character.
 */
char generateRandomChar() {
	int randomNum = rand() % 36;
	if( randomNum < 10 ) {
		return '0' + randomNum;
	} else {
		return 'A' + ( randomNum - 10 );
	}
}

/**
 * @brief Generate a transaction number based on the current time.
 *
 * @param transactionNumber The array to store the generated transaction number.
 * @param size The size of the transactionNumber array.
 */
void generateTransactionNumber( char* transactionNumber, size_t size ) {
	time_t currentTime = time( NULL );
	struct tm* timeinfo = localtime( &currentTime );
	int hour = timeinfo->tm_hour;
	int minute = timeinfo->tm_min;
	int second = timeinfo->tm_sec;
	char randomChars[4];
	for( int i = 0; i < 3; i++ ) {
		randomChars[i] = generateRandomChar();
	}
	randomChars[3] = '\0';
	snprintf( transactionNumber, size, "%c%c%c%02d%02d%02d", randomChars[0], randomChars[1], randomChars[2], hour, minute, second );
}

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
void convertDate( const char* inputDate, char* outputDate, int outputSize ) {
	int day, month, year;
	sscanf( inputDate, "%d,%d,%d", &day, &month, &year );
	struct tm dateStruct = { 0 };
	dateStruct.tm_mday = day;
	dateStruct.tm_mon = month - 1;
	dateStruct.tm_year = year - 1900;
	strftime( outputDate, outputSize, "%d %B, %Y", &dateStruct );
}

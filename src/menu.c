/**
 * @file src/menu.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/menu.h"
#include "../include/utilities.h"
#include "../include/login.h"

#define MAX_TICKET 2000
#define MAX_SHOW 50
#define MAX_USER 2500

#define SHOWS_DATABASE "data/shows.txt"
#define TICKETS_DATABASE "data/tickets.txt"

/**
 * @brief Handle navigation
 * @param userid User ID
 */
void menu( int userid ) {
    Ticket tickets[MAX_TICKET];
    Show shows[MAX_SHOW];
    printf( "\nNavigation:\n" );
    printf( "\t1. View show(s)\n" );
    printf( "\t2. Buy ticket(s)\n" );
    printf( "\t3. Cancel a ticket\n" );
    printf( "\t4. Show ticket(s)\n" );
    printf( "\t5. Exit\n" );
    printf( "Select: " );
    int selectedOption;
    scanf( "%d", &selectedOption );
    switch( selectedOption ) {
    case 1:
        printf( "\nUpcoming shows:\n" );
        viewUpcomingShows( SHOWS_DATABASE, userid, true, false, false, false );
        menu( userid );
        break;
    case 2: {
        printf( "\nAvailable show:\n" );
        int selectedShow;
        selectedShow = viewUpcomingShows( SHOWS_DATABASE, userid, true, true, false, true );
        if (selectedShow >= 0) {
            buyTicket( tickets, shows, TICKETS_DATABASE, SHOWS_DATABASE, userid, selectedShow );
        }
        menu( userid );
        break;
    }
    case 3: {
        printf( "\nAvailable tickets:\n" );
        int ticketId;
        ticketId = showTicketsByUserId( tickets, TICKETS_DATABASE, userid, true, true, false, true );
        if (ticketId >= 0) {
            updateTicketStatus( ticketId, 0 );
        }
        menu( userid );
        break;
    }
    case 4:
        printf( "\nAll your purchased tickets:\n" );
        showTicketsByUserId( tickets, TICKETS_DATABASE, userid, true, false, false, false );
        menu( userid );
        break;
    case 5:
        exit( 0 );
        break;
    default:
        exit( 0 );
    }
    return;
}

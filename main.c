/**
 * @file main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/splash.h"
#include "include/login.h"
#include "include/utilities.h"
#include "include/menu.h"

#define MAX_TICKET 100
#define MAX_SHOW 100

#define SHOWS_DATABASE "data/shows.txt"
#define TICKETS_DATABASE "data/tickets.txt"

int main() {
	splashScreen();
	int userid = login();
	if( userid >= 0 ) {
		menu( userid );
	}
	return 0;
}

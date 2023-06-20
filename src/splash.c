/**
 * @file src/splash.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/splash.h"

#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
	#include <unistd.h>
#endif

#define MAX_LINE_LENGTH 1000

#define SPLASH_DATABASE "data/splash.txt"

/**
 * @brief Splash screen
 */
void splashScreen() {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    int segmentCount = 0;
    file = fopen( SPLASH_DATABASE, "r" );
    if( file == NULL ) {
        printf( "System error.\n" );
        return;
    }
	#if defined(_WIN32) || defined(_WIN64)
		system( "cls" );
	#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
		system( "clear" );
	#endif
    while( fgets( line, sizeof( line ), file ) ) {
        size_t lineLength = strlen( line );
        for( size_t i = 0; i < lineLength; i++ ) {
            if( line[i] == '~' ) {
                fflush( stdout );
                if( segmentCount == 0 ) {
					#if defined(_WIN32) || defined(_WIN64)
						Sleep( 1000 );
						system( "cls" );
					#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
						sleep( 1 );
						system( "clear" );
					#endif
                } else if( segmentCount >= 1 && segmentCount <= 3 ) {
					#if defined(_WIN32) || defined(_WIN64)
						Sleep( 500 );
					#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
						usleep( 500000 );
					#endif
                    if( segmentCount == 3 ) {
						#if defined(_WIN32) || defined(_WIN64)
							system( "cls" );
						#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
							system( "clear" );
						#endif
                    }
                } else if( segmentCount == 4 ) {
					#if defined(_WIN32) || defined(_WIN64)
						Sleep( 1000 );
						system( "cls" );
					#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
						sleep( 1 );
						system( "clear" );
					#endif
                }
                segmentCount++;
            } else {
                printf( "%c", line[i] );
            }
        }
    }
    fclose( file );
    return;
}

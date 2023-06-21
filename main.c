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

int main() {
    splashScreen();
    int userid = login();
    if( userid >= 0 ) {
        menu( userid );
    }
    return 0;
}

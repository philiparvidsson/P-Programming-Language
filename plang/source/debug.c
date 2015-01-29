/*------------------------------------------------------------------------------
 * File: debug.c
 * Created: January 3, 2015
 * Last changed: January 11, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Makron och funktioner för debugging.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: Debug_AssertFail()
 * Parameters:
 *   expr       Uttrycket som resulterade i falskt.
 *   func_name  Namnet på den funktion som programmet kraschade i.
 *   line       Den rad i koden som programmet kraschade på.
 *
 * Description:
 *   Kraschar programmet med ett meddelande.
 *------------------------------------*/
void Debug_AssertFail(const char* expr, const char* func_name, int line) {
    printf("The assertion '%s' failed in %s() on line %d.\n"
           "This program will now crash.\n"
           "Press ENTER to continue...", expr, func_name, line);
    getchar();
    exit(EXIT_FAILURE);
}

/*--------------------------------------
 * Function: Debug_Fail()
 * Parameters:
 *   func_name  Namnet på den funktion som programmet kraschade i.
 *   line       Den rad i koden som programmet kraschade på.
 *
 * Description:
 *   Kraschar programmet med ett meddelande.
 *------------------------------------*/
void Debug_Fail(const char* func_name, int line) {
    printf("Program failed in %s() on line %d and will now crash.\n"
           "Press ENTER to continue...", func_name, line);
    getchar();
    exit(EXIT_FAILURE);
}

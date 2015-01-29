/*------------------------------------------------------------------------------
 * File: debug.h
 * Created: January 2, 2015
 * Last changed: January 13, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Makron och funktioner för debugging.
 *
 * Changes:
 *   * Lade till () efter funktionsnamnet som skrivs ut i DEBUG_PRINT.
 *   * Skapade makrot ASSERT med tillhörande funktion.
 *   * Ändrade DEBUG_PRINT()-makrot så att det visar rad i programkoden.
 *----------------------------------------------------------------------------*/

#ifndef DEBUG_H_
#define DEBUG_H_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "common.h"

#include <stdio.h>

/*------------------------------------------------
 * MACROS
 *----------------------------------------------*/

#define ASSERT(expr) if (!(expr)) {                                       \
                         Debug_AssertFail(#expr, __FUNCTION__, __LINE__); \
                     }

#define FAIL() Debug_Fail(__FUNCTION__, __LINE__)

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
void Debug_AssertFail(const char* expr, const char* func_name, int line);

/*--------------------------------------
 * Function: Debug_Fail()
 * Parameters:
 *   func_name  Namnet på den funktion som programmet kraschade i.
 *   line       Den rad i koden som programmet kraschade på.
 *
 * Description:
 *   Kraschar programmet med ett meddelande.
 *------------------------------------*/
void Debug_Fail(const char* func_name, int line);

#endif // DEBUG_H_

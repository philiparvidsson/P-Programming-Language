/*------------------------------------------------------------------------------
 * File: common.h
 * Created: January 3, 2015
 * Last changed: February 19, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Lite blandade saker som används globalt.
 *
 * Changes:
 *   * Programversion ändrad till 1.01.
 *   * Programversion ändrad till 1.03.
 *   * Programversion ändrad till 1.04.
 *   * Programversion ändrad till 1.05.
 *   * Skapade buildnum.h och inkluderade i den här filen.
 *   * Programversion ändrad till 1.06.
 *   * Programversion ändrad till 1.10.
 *----------------------------------------------------------------------------*/

#ifndef COMMON_H_
#define COMMON_H_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

// Filen 'buildnum.h' genereras vid kompilering av prebuild.bat.
#include "buildnum.h"

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: PLANG_NUM_VARS
 *
 * Description:
 *   Det maximala antalet tillåtna variabler på formen X<heltal>. Ett värde på
 *   ex. 1000 ger variabler X0 till och med X999.
 *------------------------------------*/
#define PLANG_NUM_VARS 1000 // Borde räcka för godtyckligt P-program!

/*--------------------------------------
 * Constant: PLANG_PROGRAM_AUTHORS
 *
 * Description:
 *   De programmerare som bidragit till programmets utveckling.
 *------------------------------------*/
#define PLANG_PROGRAM_AUTHORS "Philip Arvidsson (philip@philiparvidsson.com)"

/*--------------------------------------
 * Constant: PLANG_PROGRAM_VERSION
 *
 * Description:
 *   Programversionen.
 *------------------------------------*/
#define PLANG_PROGRAM_VERSION "1.10 (build "                        \
                              EXPAND_AND_STRINGIFY(PLANG_BUILD_NUM) \
                              ")"

/*------------------------------------------------
 * MACROS
 *----------------------------------------------*/

#if defined(_DEBUG) && !defined(DEBUG)
#    define DEBUG
#endif

#ifdef _MSC_VER
#    define INLINE_HINT __inline
#else
#    define INLINE_HINT inline
#endif

/*--------------------------------------
* Macro: EXPAND_AND_STRINGIFY
*
* Description:
*   Expanderar argumentet om det är ett makro, och omger resultatet med
*   citationstecken.
*------------------------------------*/
#define EXPAND_AND_STRINGIFY(s) STRINGIFY(s)

/*--------------------------------------
 * Macro: STRINGIFY
 *
 * Description:
 *   Gör en sträng av argumentet genom att omge det med citationstecken.
 *------------------------------------*/
#define STRINGIFY(s) #s

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: Bool
 *
 * Description:
 *   Boolesk typ för sant/falskt-variabler eller s.k. flaggor.
 *------------------------------------*/
typedef enum { FALSE, TRUE } Bool;

#endif // COMMON_H_

/*------------------------------------------------------------------------------
 * File: common.h
 * Created: January 3, 2015
 * Last changed: January 14, 2015
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
#define PLANG_PROGRAM_VERSION "1.06 (build "                        \
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
* Constant: EXPAND_AND_STRINGIFY
*
* Description:
*   Expanderar argumentet om det är ett makro, och omger resultatet med
*   citationstecken.
*------------------------------------*/
#define EXPAND_AND_STRINGIFY(s) STRINGIFY(s)

/*--------------------------------------
 * Constant: STRINGIFY
 *
 * Description:
 *   Gör en sträng av argumentet genom att omge det med citationstecken.
 *------------------------------------*/
#define STRINGIFY(s) #s

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

//------------------------------------------------------------------------------
// OBS! Kodsnutten nedan är från prof. Eric Roberts bibliotek, genlib.h.

/*
 * Type: Bool
 * ----------
 * This type has two values, FALSE and TRUE, which are equal to 0
 * and 1, respectively.  Most of the advantage of defining this type
 * comes from readability because it allows the programmer to
 * provide documentation that a variable will take on only one of
 * these two values.  Designing a portable representation, however,
 * is surprisingly hard, because many libraries and some compilers
 * define these names.  The definitions are usually compatible but
 * may still be flagged as errors.
 */
#ifdef THINK_C
     typedef int Bool;
#else
#    ifdef TRUE
#        ifndef Bool
#            define Bool int
#        endif
#    else
#        ifdef Bool
#            define FALSE 0
#            define TRUE  1
#        else
             typedef enum { FALSE, TRUE } Bool;
#        endif
#    endif
#endif

//------------------------------------------------------------------------------

#endif // COMMON_H_

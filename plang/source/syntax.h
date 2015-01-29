/*------------------------------------------------------------------------------
 * File: syntax.h
 * Created: January 3, 2015
 * Last changed: January 4, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Funktioner för syntax-analys av P-tokens.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifndef SYNTAX_H_
#define SYNTAX_H_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "array.h"
#include "common.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: Syntax_Error
 *
 * Description:
 *   Representerar ett fel i syntaxen hos ett P-program.
 *------------------------------------*/
typedef struct {
    char*       text;
    Bool        is_warning;
    const char* source;
    int         row;
    int         col;
} Syntax_Error;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: Syn_CheckSyntax()
 * Parameters:
 *   tokens  En array av P-tokens vars syntax ska verifieras.
 *   errors  Den array som alla eventuella fel ska lagras i.
 *   source  Källkoden, eller NULL.
 *
 * Description:
 *   Verifierar syntaxen av angivna P-tokens. Returnerar noll om syntaxen är
 *   korrekt, annars kan fel läsas ut ur errors-arrayen.
 *------------------------------------*/
Bool Syn_CheckSyntax(const Array* tokens, Array* errors, const char* source);

/*--------------------------------------
 * Function: Syn_PrintError()
 * Parameters:
 *   err  Felet som ska skrivas ut.
 *
 * Description:
 *   Skriver ut ett fel eller en varning.
 *------------------------------------*/
void Syn_PrintError(const Syntax_Error* err);

#endif // SYNTAX_H_

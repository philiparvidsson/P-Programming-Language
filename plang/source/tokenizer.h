/*------------------------------------------------------------------------------
 * File: tokenizer.h
 * Created: January 2, 2015
 * Last changed: January 4, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   I den här modulen finns funktioner som delar upp en sträng i s.k. tokens
 *   för programspråket P.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifndef TOKENIZER_H_
#define TOKENIZER_H_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "array.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: P_Token_Type
 *
 * Description:
 *   Den här enum-typen beskriver alla olika typer av s.k. tokens som får finnas
 *   i källkoden till ett program i programspråket P.
 *------------------------------------*/
typedef enum {
    PTOK_ASSIGN,  // :=
    PTOK_COMMA,   // ,
    PTOK_DO,      // DO
    PTOK_END,     // END
    PTOK_EOF,     // I slutet finns alltid en EOF-token.
    PTOK_EQ_TEST, // !=
    PTOK_IDENT,   // X0, X1, X2, ... etc.
    PTOK_INT,     // 0, 1, 2, ... etc.
    PTOK_L_PAREN, // (
    PTOK_PRED,    // PRED
    PTOK_PROGRAM, // PROGRAM
    PTOK_RESULT,  // RESULT
    PTOK_R_PAREN, // )
    PTOK_SUCC,    // SUCC
    PTOK_UNKNOWN, // Okänd token.
    PTOK_WHILE    // WHILE
} P_Token_Type;

/*--------------------------------------
 * Type: P_Token
 *
 * Description:
 *   Den här typen beskriver en s.k. token i programspråket P.
 *------------------------------------*/
typedef struct {
    P_Token_Type type;
    int          row;
    int          col;
    char*        value;
} P_Token;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: Tok_GetString()
 * Parameters:
 *   tok  Den token som ska omvandlas till en sträng.
 *
 * Description:
 *   Den här funktionen gör om en token till en sträng som går att läsa.
 *------------------------------------*/
const char* Tok_GetString(const P_Token* tok);

/*--------------------------------------
 * Function: Tok_Tokenize()
 * Parameters:
 *   src     En sträng som innehåller källkoden till ett program i
 *           programspråket P.
 *   tokens  Den array som alla s.k. tokens ska lagras i.
 *
 * Description:
 *   Den här funktionen läser av källkod skriven i programspråket P och delar
 *   upp den i en array av tokens.
 *------------------------------------*/
void Tok_Tokenize(const char* src, Array* tokens);

#endif // TOKENIZER_H_

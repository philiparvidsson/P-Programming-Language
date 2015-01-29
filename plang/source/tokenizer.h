/*------------------------------------------------------------------------------
 * File: tokenizer.h
 * Created: January 2, 2015
 * Last changed: January 4, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   I den h�r modulen finns funktioner som delar upp en str�ng i s.k. tokens
 *   f�r programspr�ket P.
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
 *   Den h�r enum-typen beskriver alla olika typer av s.k. tokens som f�r finnas
 *   i k�llkoden till ett program i programspr�ket P.
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
    PTOK_UNKNOWN, // Ok�nd token.
    PTOK_WHILE    // WHILE
} P_Token_Type;

/*--------------------------------------
 * Type: P_Token
 *
 * Description:
 *   Den h�r typen beskriver en s.k. token i programspr�ket P.
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
 *   tok  Den token som ska omvandlas till en str�ng.
 *
 * Description:
 *   Den h�r funktionen g�r om en token till en str�ng som g�r att l�sa.
 *------------------------------------*/
const char* Tok_GetString(const P_Token* tok);

/*--------------------------------------
 * Function: Tok_Tokenize()
 * Parameters:
 *   src     En str�ng som inneh�ller k�llkoden till ett program i
 *           programspr�ket P.
 *   tokens  Den array som alla s.k. tokens ska lagras i.
 *
 * Description:
 *   Den h�r funktionen l�ser av k�llkod skriven i programspr�ket P och delar
 *   upp den i en array av tokens.
 *------------------------------------*/
void Tok_Tokenize(const char* src, Array* tokens);

#endif // TOKENIZER_H_

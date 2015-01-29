/*------------------------------------------------------------------------------
 * File: tokenizer.c
 * Created: January 2, 2015
 * Last changed: January 14, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   I den h�r modulen finns funktioner som delar upp en str�ng i s.k. tokens
 *   f�r programspr�ket P.
 *
 * Changes:
 *   * Tokeniseraren skiljer inte l�ngre p� gemener och versaler.
 *   * Anv�nder Str_CompareI() f�r att hitta nyckelord ist�llet f�r case-satser.
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "array.h"
#include "common.h"
#include "string.h"
#include "tokenizer.h"

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
const char* Tok_GetString(const P_Token* tok) {
    // Variabler och heltal returnerar vi som str�ngar, s� att man kan se deras
    // inneh�ll direkt.
    if (tok->type == PTOK_IDENT || tok->type == PTOK_INT)
        return tok->value;

    switch (tok->type) {
        case PTOK_ASSIGN : return ":=";
        case PTOK_COMMA  : return ",";
        case PTOK_DO     : return "DO";
        case PTOK_END    : return "END";
        case PTOK_EOF    : return "<eof>";
        case PTOK_EQ_TEST: return "!=";
        case PTOK_L_PAREN: return "(";
        case PTOK_PRED   : return "PRED";
        case PTOK_PROGRAM: return "PROGRAM";
        case PTOK_RESULT : return "RESULT";
        case PTOK_R_PAREN: return ")";
        case PTOK_SUCC   : return "SUCC";
        case PTOK_UNKNOWN: return "<unknown>";
        case PTOK_WHILE  : return "WHILE";
    }

    return "<invalid>";
}

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
void Tok_Tokenize(const char* src, Array* tokens) {
    int row = 1;
    int col = 1;

    while (TRUE) {
        char    c   = *(src++);
        P_Token tok = { .type  = PTOK_UNKNOWN,
                        .value = NULL,
                        .col   = col++,
                        .row   = row };

        if (c == '\0') {
            tok.type = PTOK_EOF;
            Array_AddElem(tokens, &tok);
            break;
        }

        switch (c) {
        /*----------------------------------------------------
         * Ignorera mellanslag, nya rader osv.
         *--------------------------------------------------*/
        case '\r':
        case ' ':
            continue;
        case '\n':
            row++;
            col = 1;
            continue;
        case '\t':
            // Vi antar att tab �r 8 tecken bred, men adderar endast 7 eftersom
            // vi har col++ ovan. (tok.col = col++).
            col += 7;
            continue;

        /*----------------------------------------------------
         * Kommentar.
         *--------------------------------------------------*/
        case '#':
            while (*src && *src != '\n')
                src++;
            continue;

        /*----------------------------------------------------
         * Komma.
         *--------------------------------------------------*/
        case ',': tok.type = PTOK_COMMA; break;

        /*----------------------------------------------------
         * Parenteser.
         *--------------------------------------------------*/
        case '(': tok.type = PTOK_L_PAREN; break;
        case ')': tok.type = PTOK_R_PAREN; break;
        
        /*----------------------------------------------------
         * F�rmodligen tilldelning. (:=)
         *--------------------------------------------------*/
        case ':':
            c = *src;
            if (c == '=') {
                tok.type = PTOK_ASSIGN;
                src++; col++;
            }
            break;
        
        /*----------------------------------------------------
         * F�rmodligen ekvivalenstest. (!=)
         *--------------------------------------------------*/
        case '!':
            c = *src;
            if (c == '=') {
                tok.type = PTOK_EQ_TEST;
                src++; col++;
            }
            break;

        /*----------------------------------------------------
         * Nyckelord, variabel, heltal...
         *--------------------------------------------------*/
        default:
            if (!Chr_IsAlphaNum(c)) break;

            char buf[1024];
            int  i = 1;

            buf[0] = c;
            while ((c = *src) && Chr_IsAlphaNum(c)) {
                src++; col++;
                if (i < (sizeof(buf)-1))
                    buf[i++] = c;
            }

            buf[i] = '\0';

                 if (Str_CompareI(buf, "DO"     )==0) tok.type = PTOK_DO;
            else if (Str_CompareI(buf, "END"    )==0) tok.type = PTOK_END;
            else if (Str_CompareI(buf, "PRED"   )==0) tok.type = PTOK_PRED;
            else if (Str_CompareI(buf, "PROGRAM")==0) tok.type = PTOK_PROGRAM;
            else if (Str_CompareI(buf, "RESULT" )==0) tok.type = PTOK_RESULT;
            else if (Str_CompareI(buf, "SUCC"   )==0) tok.type = PTOK_SUCC;
            else if (Str_CompareI(buf, "WHILE"  )==0) tok.type = PTOK_WHILE;
            else if (Chr_ToLower(buf[0])=='x') {
                tok.type  = PTOK_IDENT;
                tok.value = Str_Duplicate(buf);
            }
            else if (Str_IsNumeric(buf)) {
                tok.type  = PTOK_INT;
                tok.value = Str_Duplicate(buf);
            }
        }

        Array_AddElem(tokens, &tok);
    } // while (TRUE)
}

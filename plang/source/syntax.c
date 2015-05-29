/*------------------------------------------------------------------------------
 * File: syntax.c
 * Created: January 3, 2015
 * Last changed: February 19, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Funktioner för syntax-analys av P-tokens. Den här modulen står för syntax-
 *   analys, generering av felmeddelanden, m.m. Syftet är att kunna verifiera
 *   (den syntaktiska) korrekheten hos en angiven P-kod (i form av en mängd
 *   tokens).
 *
 * Changes:
 *   * Kör numer Str_Duplicate() för alla strängar i felmeddelanden, så att man
 *     kan köra free() på dem rakt igenom.
 *   * Tog bort felmeddelandet vill tilldelning av andra värden än än noll och
 *     ersatte det med en varning istället. Tilldelning av andra värdeän än noll
 *     är nu tillåtna.
 *   * Felkontroll för heltal.
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "array.h"
#include "common.h"
#include "debug.h"
#include "string.h"
#include "syntax.h"
#include "tokenizer.h"
#include "vm.h"

#include <stdio.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: MAX_ERRORS
 *
 * Description:
 *   Det maximala antalet fel vi accepterar innan vi avslutar syntax-
 *   verifieringen. Detta är i själva verket inte en stenhård gräns, utan
 *   värdet kan passeras något beroende på var i syntax-verifieringen programmet
 *   befinner sig. Tanken är att förhindra att den spottar ur sig flera hundra
 *   felmeddelanden åt gången.
 *------------------------------------*/
#define MAX_ERRORS 10

/*--------------------------------------
 * Constant: MAX_IDENT_LEN
 *
 * Description:
 *   Det maximala tillåtna antalet tecken på variabelnamn. Vi kan använda
 *   MAX_INT_LEN+1 eftersom variabelnamn är ett X följt av ett heltal.
 *------------------------------------*/
#define MAX_IDENT_LEN (MAX_INT_LEN+1)

/*--------------------------------------
 * Constant: MAX_INT_LEN
 *
 * Description:
 *   Det maximala tillåtna antalet tecken som heltal får innehålla
 *------------------------------------*/
#define MAX_INT_LEN 11

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: ErrInvalidIdent()
 * Parameters:
 *   errors  Den array som felet ska läggas i.
 *   source  Källkoden, eller NULL.
 *   tok     Den token som felet upptäcktes vid.
 *
 * Description:
 *   Genererar ett fel om att en ogiltig variabel används i källkoden.
 *------------------------------------*/
static void ErrInvalidIdent(Array* errors, const char* source,
                            const P_Token* tok)
{
    Syntax_Error error;

    // Vi använder Str_Duplicate här så att det är ok att köra free() på alla
    // felmeddelanden.
    error.text       = Str_Duplicate("invalid identifier");
    error.is_warning = FALSE;
    error.source     = source;
    error.row        = tok->row;
    error.col        = tok->col;

    Array_AddElem(errors, &error);
}

/*--------------------------------------
 * Function: ErrInvalidInt()
 * Parameters:
 *   errors  Den array som felet ska läggas i.
 *   source  Källkoden, eller NULL.
 *   tok     Den token som felet upptäcktes vid.
 *
 * Description:
 *   Genererar ett fel om att ett ogiltigt heltal används i källkoden.
 *------------------------------------*/
static void ErrInvalidInt(Array* errors, const char* source,
                          const P_Token* tok)
{
    Syntax_Error error;

    // Vi använder Str_Duplicate här så att det är ok att köra free() på alla
    // felmeddelanden.
    error.text       = Str_Duplicate("invalid integer");
    error.is_warning = FALSE;
    error.source     = source;
    error.row        = tok->row;
    error.col        = tok->col;

    Array_AddElem(errors, &error);
}

/*--------------------------------------
 * Function: ErrLoopTestAgainstNonZero()
 * Parameters:
 *   errors  Den array som felet ska läggas i.
 *   source  Källkoden, eller NULL.
 *   tok     Den token som felet upptäcktes vid.
 *
 * Description:
 *   Genererar ett fel om att man måste testa mot noll i villkoret för while-
 *   loopar.
 *------------------------------------*/
static void ErrLoopTestAgainstNonZero(Array* errors, const char* source,
                                      const P_Token* tok)
{
    Syntax_Error error;

    // Vi använder Str_Duplicate här så att det är ok att köra free() på alla
    // felmeddelanden.
    error.text       = Str_Duplicate("this must be zero");
    error.is_warning = FALSE;
    error.source     = source;
    error.row        = tok->row;
    error.col        = tok->col;

    Array_AddElem(errors, &error);
}

/*--------------------------------------
 * Function: ErrUnexpectedToken()
 * Parameters:
 *   errors  Den array som felet ska läggas i.
 *   source  Källkoden, eller NULL.
 *   tok     Den token som felet upptäcktes vid.
 *
 * Description:
 *   Genererar ett fel om att en okänd s.k. token påträffades.
 *------------------------------------*/
static void ErrUnexpectedToken(Array* errors, const char* source,
                               const P_Token* tok)
{
    char         buf[1024];
    Syntax_Error error;

    sprintf(buf, "unexpected token: %s", Tok_GetString(tok));

    error.text       = Str_Duplicate(buf);
    error.is_warning = FALSE;
    error.source     = source;
    error.row        = tok->row;
    error.col        = tok->col;

    Array_AddElem(errors, &error);
}

/*--------------------------------------
 * Function: ExpectToken()
 * Parameters:
 *   errors    Den array som felet ska läggas i.
 *   source    Källkoden, eller NULL.
 *   tok       Den token som felet upptäcktes vid.
 *   expected  Den token-typ som förväntas.
 *
 * Description:
 *   Genererar ett fel om den angivna token inte motsvarar den förväntade typen.
 *------------------------------------*/
static Bool ExpectToken(Array* errors, const char* source, const P_Token* tok,
                        P_Token_Type expected)
{
    if (tok->type == expected)
        return TRUE;

    char         buf[1024];
    Syntax_Error error;
    const char*  str;
    P_Token      tmp_tok = { .type = expected };

         if (expected == PTOK_IDENT) { str = "identifier";            }
    else if (expected == PTOK_INT)   { str = "integer";               }
    else                             { str = Tok_GetString(&tmp_tok); }

    sprintf(buf, "expected %s but got %s", str, Tok_GetString(tok));

    error.text       = Str_Duplicate(buf);
    error.is_warning = FALSE;
    error.source     = source;
    error.row        = tok->row;
    error.col        = tok->col;

    Array_AddElem(errors, &error);

    return FALSE;
}

/*--------------------------------------
 * Function: ExpectToken2()
 * Parameters:
 *   errors     Den array som felet ska läggas i.
 *   source     Källkoden, eller NULL.
 *   tok        Den token som felet upptäcktes vid.
 *   expected1  Den första token-typen som förväntas.
 *   expected2  Den andra token-typen som förväntas.
 *
 * Description:
 *   Genererar ett fel om den angivna token inte motsvarar någon av de
 *   förväntade typerna.
 *------------------------------------*/
static Bool ExpectToken2(Array* errors, const char* source, const P_Token* tok,
                         P_Token_Type expected1, P_Token_Type expected2)
{
    if (tok->type == expected1 || tok->type == expected2)
        return TRUE;

    char         buf[1024];
    Syntax_Error error;
    const char*  str1;
    const char*  str2;
    P_Token      tmp_tok1 = { .type = expected1 };
    P_Token      tmp_tok2 = { .type = expected2 };
    
         if (expected1 == PTOK_IDENT) { str1 = "identifier";             }
    else if (expected1 == PTOK_INT)   { str1 = "integer";                }
    else                              { str1 = Tok_GetString(&tmp_tok1); }

         if (expected2 == PTOK_IDENT) { str2 = "identifier";             }
    else if (expected2 == PTOK_INT)   { str2 = "integer";                }
    else                              { str2 = Tok_GetString(&tmp_tok2); }

    sprintf(buf, "expected %s or %s but got %s", str1, str2,
            Tok_GetString(tok));

    error.text       = Str_Duplicate(buf);
    error.is_warning = FALSE;
    error.source     = source;
    error.row        = tok->row;
    error.col        = tok->col;

    Array_AddElem(errors, &error);

    return FALSE;
}

/*--------------------------------------
 * Function: ExpectToken3()
 * Parameters:
 *   errors     Den array som felet ska läggas i.
 *   source     Källkoden, eller NULL.
 *   tok        Den token som felet upptäcktes vid.
 *   expected1  Den första token-typen som förväntas.
 *   expected2  Den andra token-typen som förväntas.
 *   expected3  Den tredje token-typen som förväntas.
 *
 * Description:
 *   Genererar ett fel om den angivna token inte motsvarar någon av de
 *   förväntade typerna.
 *------------------------------------*/
static Bool ExpectToken3(Array* errors, const char* source, const P_Token* tok,
                         P_Token_Type expected1, P_Token_Type expected2,
                         P_Token_Type expected3)
{
    if (tok->type == expected1 || tok->type == expected2
     || tok->type == expected3)
    {
        return TRUE;
    }

    char         buf[1024];
    Syntax_Error error;
    const char*  str1;
    const char*  str2;
    const char*  str3;
    P_Token      tmp_tok1 = { .type = expected1 };
    P_Token      tmp_tok2 = { .type = expected2 };
    P_Token      tmp_tok3 = { .type = expected3 };

         if (expected1 == PTOK_IDENT) { str1 = "identifier";             }
    else if (expected1 == PTOK_INT)   { str1 = "integer";                }
    else                              { str1 = Tok_GetString(&tmp_tok1); }

         if (expected2 == PTOK_IDENT) { str2 = "identifier";             }
    else if (expected2 == PTOK_INT)   { str2 = "integer";                }
    else                              { str2 = Tok_GetString(&tmp_tok2); }

         if (expected3 == PTOK_IDENT) { str3 = "identifier";             }
    else if (expected3 == PTOK_INT)   { str3 = "integer";                }
    else                              { str3 = Tok_GetString(&tmp_tok3); }

    sprintf(buf, "expected %s or %s or %s but got %s", str1, str2, str3,
            Tok_GetString(tok));

    error.text       = Str_Duplicate(buf);
    error.is_warning = FALSE;
    error.source     = source;
    error.row        = tok->row;
    error.col        = tok->col;

    Array_AddElem(errors, &error);

    return FALSE;
}

/*--------------------------------------
 * Function: WarnEmptyLoop()
 * Parameters:
 *   errors  Den array som varningen ska läggas i.
 *   source  Källkoden, eller NULL.
 *   tok     Den token som felet upptäcktes vid.
 *
 * Description:
 *   Genererar en varning om tom while-loop.
 *------------------------------------*/
static void WarnEmptyLoop(Array* errors, const char* source,
                          const P_Token* tok)
{
    Syntax_Error error;

    // Vi använder Str_Duplicate här så att det är ok att köra free() på alla
    // felmeddelanden.
    error.text       = Str_Duplicate("loop is empty and will never finish");
    error.is_warning = TRUE;
    error.source     = source;
    error.row        = tok->row;
    error.col        = tok->col;

    Array_AddElem(errors, &error);
}

/*--------------------------------------
 * Function: WarnAssignNonZero()
 * Parameters:
 *   errors  Den array som felet ska läggas i.
 *   source  Källkoden, eller NULL.
 *   tok     Den token som felet upptäcktes vid.
 *
 * Description:
 *   Genererar en varning om att man tilldelar något annat än noll. Språket P
 *   gillar egentligen inte det.
 *------------------------------------*/
static void WarnAssignNonZero(Array* errors, const char* source,
                              const P_Token* tok)
{
    Syntax_Error error;

    // Vi använder Str_Duplicate här så att det är ok att köra free() på alla
    // felmeddelanden.
    error.text       = Str_Duplicate("assigned value is non-zero");
    error.is_warning = TRUE;
    error.source     = source;
    error.row        = tok->row;
    error.col        = tok->col;

    Array_AddElem(errors, &error);
}

/*--------------------------------------
 * Function: WarnInfiniteLoop()
 * Parameters:
 *   errors  Den array som varningen ska läggas i.
 *   source  Källkoden, eller NULL.
 *   tok     Den token som felet upptäcktes vid.
 *
 * Description:
 *   Genererar en varning om oändlig while-loop.
 *------------------------------------*/
static void WarnInfiniteLoop(Array* errors, const char* source,
                             const P_Token* tok)
{
    char         buf[1024];
    Syntax_Error error;

    sprintf(buf, "the conditional loop var %s is never modified", tok->value);

    error.text       = Str_Duplicate(buf);
    error.is_warning = TRUE;
    error.source     = source;
    error.row        = tok->row;
    error.col        = tok->col;

    Array_AddElem(errors, &error);
}

/*--------------------------------------
 * Function: CheckIdent()
 * Parameters:
 *   errors     Den array som alla eventuella fel ska lagras i.
 *   source     Källkoden, eller NULL.
 *   ident_tok  Den token som innehåller variabelnamnet.
 *
 * Description:
 *   Verifierar att den angivna token representerar en giltig variabel.
 *------------------------------------*/
static void CheckIdent(Array* errors, const char* source,
                       const P_Token* ident_tok)
{
    if (!ident_tok->value) {
        ErrInvalidIdent(errors, source, ident_tok);
        return;
    }

    int len = strlen(ident_tok->value);
    if (len<2 || len>MAX_IDENT_LEN) {
        ErrInvalidIdent(errors, source, ident_tok);
        return;
    }

    if (Chr_ToLower(ident_tok->value[0]) != 'x') {
        ErrInvalidIdent(errors, source, ident_tok);
        return;
    }

    if (!Str_IsNumeric(ident_tok->value+1)) {
        ErrInvalidIdent(errors, source, ident_tok);
        return;
    }

    // Talet i variabelns namn måste referera en giltig plats i minnet.
    int var = atoi(ident_tok->value+1);
    if (var < 0 || var >= PLANG_NUM_VARS)
        ErrInvalidIdent(errors, source, ident_tok);
}

/*--------------------------------------
 * Function: CheckInt()
 * Parameters:
 *   errors   Den array som alla eventuella fel ska lagras i.
 *   source   Källkoden, eller NULL.
 *   int_tok  Den token som innehåller heltalet.
 *
 * Description:
 *   Verifierar att den angivna token representerar ett giltigt heltal.
 *------------------------------------*/
static void CheckInt(Array* errors, const char* source,
                     const P_Token* int_tok)
{
    if (!int_tok->value) {
        ErrInvalidInt(errors, source, int_tok);
        return;
    }

    int len = strlen(int_tok->value);
    if (len<1 || len>MAX_INT_LEN) {
        ErrInvalidInt(errors, source, int_tok);
        return;
    }

    if (!Str_IsNumeric(int_tok->value))
        ErrInvalidInt(errors, source, int_tok);
}

/*--------------------------------------
 * Function: CheckLoopFinite()
 * Parameters:
 *   index      Det index i token-arrayen som verifieringen ska börja på.
 *   errors     Den array som alla eventuella fel ska lagras i.
 *   source     Källkoden, eller NULL.
 *   while_tok  Den token som innehåller loop-variabeln.
 *
 * Description:
 *   Verifierar att loop-variabeln modifieras inuti while-loopen. Annars
 *   genereras en varning om detta.
 *------------------------------------*/
static void CheckLoopFinite(int index, const Array* tokens, Array* errors,
                            const char* source, P_Token* while_tok)
{
    Bool is_infinite_loop = TRUE;
    int  num_nested_loops = 0;
    int  num_tokens       = Array_Length(tokens);
    int  while_var        = atoi(while_tok->value+1);

    // Här struntar vi i syntaxen. Vi loopar bara fram till END och kollar att
    // vi hittar en token som indikerar tilldelning av loop-variabeln.
    while (TRUE) {
        if (index >= num_tokens) return;
        P_Token* tok = Array_GetElemPtr(tokens, index++);

        if (tok->type == PTOK_ASSIGN) {
            // Endast := kan leda till att loopen tar slut.
            tok = Array_GetElemPtr(tokens, index-2);
            // tok = den token som innehåller variabeln som tilldelas.

            if (tok->type != PTOK_IDENT) {
                // Trasig syntax, men det reder vi inte ut här.
                continue;
            }

            int var = atoi(tok->value+1);
            if (var != while_var) {
                // Vi har en tilldelning, men den gäller inte variabeln i loop-
                // villkoret.
                continue;
            }

            if (index >= num_tokens) return;
            P_Token* int_pred_succ_tok = Array_GetElemPtr(tokens, index);

            if (int_pred_succ_tok->type == PTOK_INT) {
                // Om det inte är värdet noll vi tilldelar så kan tilldelningen
                // inte stanna loopen.
                int val = atoi(int_pred_succ_tok->value);
                if (val > 0)
                    continue;
            }
            else if (int_pred_succ_tok->type == PTOK_SUCC) {
                // SUCC kan inte stoppa loopen annat än genom overflow, i vilket
                // fall den virtuella maskinen ändå avbryter exekveringen.
                continue;
            }

            // Tilldelningen är antingen <variabel> := PRED... eller
            // <variabel> := 0.

            is_infinite_loop = FALSE;
            break;
        }
        else if (tok->type == PTOK_WHILE) {
            // Vi har hamnat i en while-loop inuti den vi kontrollerar.
            num_nested_loops++;
        }
        else if (tok->type == PTOK_END) {
            // Vi har nått loopens syntaktiska slut om nesting blir noll här.
            if (num_nested_loops-- <= 0)
                break;
        }
    }

    if (is_infinite_loop)
        WarnInfiniteLoop(errors, source, while_tok);
}

/*--------------------------------------
 * Function: CheckSyntax()
 * Parameters:
 *   index   Det index i token-arrayen som verifieringen ska börja på.
 *   tokens  En array av P-tokens vars syntax ska verifieras.
 *   errors  Den array som alla eventuella fel ska lagras i.
 *   source  Källkoden, eller NULL.
 *
 * Description:
 *   Verifierar syntaxen av angivna P-tokens. Returnerar noll om syntaxen är
 *   korrekt, annars kan fel läsas ut ur errors-arrayen.
 *------------------------------------*/
static void CheckSyntax(int* index, const Array* tokens, Array* errors,
                        const char* source)
{
    int num_tokens = Array_Length(tokens);
    while (TRUE) {
        // Om det genererats för många fel så ger vi upp och avslutar syntax-
        // verifieringen.
        int num_errors = Array_Length(errors);
        if (num_errors >= MAX_ERRORS) {
            // Fel kan även vara varningar, så vi räknar antalet egentliga fel.
            int num_actual_errors = 0;

            for (int i = 0; i < num_errors; i++) {
                Syntax_Error* err = Array_GetElemPtr(errors, i);

                if (!err->is_warning)
                    num_actual_errors++;
            }

            // För många fel (varningar ej inräknade), så vi ger upp här.
            if (num_actual_errors >= MAX_ERRORS) {
                *index = num_tokens;
                return;
            }
        }

        if (*index >= num_tokens) return;
        P_Token* tok = Array_GetElemPtr(tokens, (*index)++);

        switch (tok->type) {
        /*----------------------------------------------------
         * <variabel> := <naturligt-tal>
         * <variabel> := PRED(<variabel>)
         * <variabel> := SUCC(<variabel>)
         *--------------------------------------------------*/
        case PTOK_IDENT: {
            CheckIdent(errors, source, tok);

            if (*index >= num_tokens) return;
            tok = Array_GetElemPtr(tokens, (*index)++);
            ExpectToken(errors, source, tok, PTOK_ASSIGN);

            if (*index >= num_tokens) return;
            tok = Array_GetElemPtr(tokens, (*index)++);
            ExpectToken3(errors, source, tok, PTOK_INT, PTOK_PRED, PTOK_SUCC);

            if (tok->type == PTOK_INT) {
                // <variabel> := <naturligt-tal>

                CheckInt(errors, source, tok);

                char* s = tok->value;
                while (s && *s) {
                    if (*(s++) != '0') {
                        // Tilldelning av något annat än noll.
                        WarnAssignNonZero(errors, source, tok);
                        break;
                    }
                }
            }
            else if (tok->type == PTOK_PRED || tok->type == PTOK_SUCC) {
                // <variabel> := PRED(<variabel>)
                // <variabel> := SUCC(<variabel>)

                if (*index >= num_tokens) return;
                tok = Array_GetElemPtr(tokens, (*index)++);
                ExpectToken(errors, source, tok, PTOK_L_PAREN);

                if (*index >= num_tokens) return;
                tok = Array_GetElemPtr(tokens, (*index)++);
                ExpectToken(errors, source, tok, PTOK_IDENT);

                CheckIdent(errors, source, tok);

                if (*index >= num_tokens) return;
                tok = Array_GetElemPtr(tokens, (*index)++);
                ExpectToken(errors, source, tok, PTOK_R_PAREN);
            }

            break;
        }

        /*----------------------------------------------------
         * WHILE <variabel> != 0 DO ... END
         *--------------------------------------------------*/
        case PTOK_WHILE: {
            if (*index >= num_tokens) return;
            tok = Array_GetElemPtr(tokens, (*index)++);
            ExpectToken(errors, source, tok, PTOK_IDENT);

            CheckIdent(errors, source, tok);

            // Vi sparar denna token så att vi kan kontrollera om loop-variabeln
            // modifieras inuti loopen. Annars tar ju loopen aldrig slut.
            P_Token* while_tok = tok;

            if (*index >= num_tokens) return;
            tok = Array_GetElemPtr(tokens, (*index)++);
            ExpectToken(errors, source, tok, PTOK_EQ_TEST);

            if (*index >= num_tokens) return;
            tok = Array_GetElemPtr(tokens, (*index)++);
            ExpectToken(errors, source, tok, PTOK_INT);

            CheckInt(errors, source, tok);

            // Se till att vi testar mot värdet noll.
            char* s = tok->value;
            while (s && *s) {
                if (*(s++) != '0') {
                    // Ogiltigt försök att testa mot något annat värde än noll.
                    ErrLoopTestAgainstNonZero(errors, source, tok);
                    break;
                }
            }

            if (*index >= num_tokens) return;
            tok = Array_GetElemPtr(tokens, (*index)++);
            ExpectToken(errors, source, tok, PTOK_DO);

            if (*index >= num_tokens) return;
            tok = Array_GetElemPtr(tokens, *index);
            if (tok->type != PTOK_END) {
                CheckLoopFinite(*index, tokens, errors, source, while_tok);

                // Här går vi igenom loopens innehåll med hjälp av rekursion.
                CheckSyntax(index, tokens, errors, source);

                if (*index >= num_tokens) return;
                tok = Array_GetElemPtr(tokens, (*index)++);
                ExpectToken(errors, source, tok, PTOK_END);
            }
            else {
                // Loopen är tom, vilket ju är syntaktiskt ok, men den kommer
                // aldrig bli klar, så vi genererar en varning om det.
                WarnEmptyLoop(errors, source, tok);
                (*index)++;
            }

            break;
        }

        /*----------------------------------------------------
         * END
         *--------------------------------------------------*/
        case PTOK_END: {
            // Detta är slutet på en while-loop.
            (*index)--;
            return;
        }

        /*----------------------------------------------------
         * RESULT (<variabel>)
         *--------------------------------------------------*/
        case PTOK_RESULT: {
            if (*index >= num_tokens) return;
            tok = Array_GetElemPtr(tokens, (*index)++);
            ExpectToken(errors, source, tok, PTOK_L_PAREN);

            if (*index >= num_tokens) return;
            tok = Array_GetElemPtr(tokens, (*index)++);
            ExpectToken(errors, source, tok, PTOK_IDENT);

            CheckIdent(errors, source, tok);

            if (*index >= num_tokens) return;
            tok = Array_GetElemPtr(tokens, (*index)++);
            ExpectToken(errors, source, tok, PTOK_R_PAREN);

            if (*index >= num_tokens) return;
            tok = Array_GetElemPtr(tokens, *index);
            ExpectToken(errors, source, tok, PTOK_EOF);
        
            // Här är vi klara med verifieringen!
            return;
        }

        default:
            ErrUnexpectedToken(errors, source, tok);
        }
    }

    // Hit kommer vi aldrig.
    FAIL();
}

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
Bool Syn_CheckSyntax(const Array* tokens, Array* errors, const char* source) {
    int index      = 0;
    int num_tokens = Array_Length(tokens);

    /*----------------------------------------------------
     * Alla program börjar med följande sekvens:
     *   PROGRAM (<variabel>[, <variabel>])
     *--------------------------------------------------*/

    if (index >= num_tokens) return FALSE;
    P_Token* tok = Array_GetElemPtr(tokens, index++);
    ExpectToken(errors, source, tok, PTOK_PROGRAM);

    if (index >= num_tokens) return FALSE;
    tok = Array_GetElemPtr(tokens, index++);
    ExpectToken(errors, source, tok, PTOK_L_PAREN);

    while (TRUE) {
        if (index >= num_tokens) return FALSE;
        tok = Array_GetElemPtr(tokens, index++);
        ExpectToken(errors, source, tok, PTOK_IDENT);

        CheckIdent(errors, source, tok);

        if (index >= num_tokens) return FALSE;
        tok = Array_GetElemPtr(tokens, index);
        if (tok->type != PTOK_COMMA)
            break;
        index++;
    }

    if (index >= num_tokens) return FALSE;
    tok = Array_GetElemPtr(tokens, index++);
    ExpectToken2(errors, source, tok, PTOK_COMMA, PTOK_R_PAREN);

    // Vi fortsätter djupare in i programmet och verifierar syntaxen där.
    CheckSyntax(&index, tokens, errors, source);

    int num_errors = Array_Length(errors);
    for (int i = 0; i < num_errors; i++) {
        Syntax_Error* err = Array_GetElemPtr(errors, i);

        // Varningar gör inte så mycket, men fel kan vi inte acceptera.
        if (!err->is_warning)
            return FALSE;
    }

    // Allt gick som det skulle! :-)
    return TRUE;
}

/*--------------------------------------
 * Function: Syn_PrintError()
 * Parameters:
 *   err  Felet som ska skrivas ut.
 *
 * Description:
 *   Skriver ut ett fel eller en varning.
 *------------------------------------*/
void Syn_PrintError(const Syntax_Error* err) {
    if (err->is_warning) printf("Warning on line %d:\n", err->row);
    else                 printf("Error on line %d:\n"  , err->row);

    // "Spola fram" till rätt rad i källkoden.
    const char* source = err->source;
    int row = err->row-1;
    while (row > 0) {
        if (*(source++) == '\n')
            row--;
    }

    // Fult hack för att skriva ut textraden, men det fungerar fint.
    while (TRUE) {
        char c = *(source++);
        if (c=='\r' || c=='\n' || c=='\0')
            break;
        putchar(c);
    }

    printf("\n");

    // Vi markerar exakt var felet uppstod.
    for (int i = 1; i < err->col; i++)
        printf(" ");
    printf("^-- %s\n", err->text);
}

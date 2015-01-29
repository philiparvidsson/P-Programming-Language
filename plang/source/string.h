/*------------------------------------------------------------------------------
 * File: string.h
 * Created: January 14, 2015
 * Last changed: January 14, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Sträng-funktioner.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifndef STRING_H_
#define STRING_H_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "common.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: Chr_IsAlphaNum()
 * Parameters:
 *   c  Ett tecken.
 *
 * Description:
 *   Returnerar sant om det angivna tecknet är alfanumeriskt.
 *------------------------------------*/
Bool Chr_IsAlphaNum(char c);

/*--------------------------------------
 * Function: Chr_IsAlpha()
 * Parameters:
 *   c  Ett tecken.
 *
 * Description:
 *   Returnerar sant om det angivna tecknet är alfabetiskt.
 *------------------------------------*/
Bool Chr_IsAlpha(char c);

/*--------------------------------------
 * Function: Chr_IsDigit()
 * Parameters:
 *   c  Ett tecken.
 *
 * Description:
 *   Returnerar sant om det angivna tecknet är numeriskt.
 *------------------------------------*/
Bool Chr_IsDigit(char c);

/*--------------------------------------
 * Function: Chr_ToLower()
 * Parameters:
 *   c  Ett tecken.
 *
 * Description:
 *   Returnerar sant om det angivna tecknet som gemen.
 *------------------------------------*/
char Chr_ToLower(char c);

/*--------------------------------------
 * Function: Str_Compare()
 * Parameters:
 *   s1  Sträng ett.
 *   s2  Sträng två.
 *
 * Description:
 *   Jämför de angivna strängarna och returnerar noll om de är identiska.
 *------------------------------------*/
int Str_Compare(const char* s1, const char* s2);

/*--------------------------------------
 * Function: Str_CompareI()
 * Parameters:
 *   s1  Sträng ett.
 *   s2  Sträng två.
 *
 * Description:
 *   Jämför de angivna strängarna utan att ta hänsyn till gemener/versaler och
 *   returnerar noll om de i övrigt är identiska.
 *------------------------------------*/
int Str_CompareI(const char* s1, const char* s2);

/*--------------------------------------
 * Function: Str_Duplicate()
 * Parameters:
 *   s  En sträng.
 *
 * Description:
 *   Duplicerar den angivna strängen i minnet. Glöm inte anropa free()!
 *------------------------------------*/
char* Str_Duplicate(const char* s);

/*--------------------------------------
 * Function: Str_IsAlpha()
 * Parameters:
 *   s  En sträng.
 *
 * Description:
 *   Returnerar sant om strängen endast innehåller alfabetiska tecken.
 *------------------------------------*/
Bool Str_IsAlpha(const char* s);

/*--------------------------------------
 * Function: Str_IsAlphaNum()
 * Parameters:
 *   s  En sträng.
 *
 * Description:
 *   Returnerar sant om strängen endast innehåller alfanumeriska tecken.
 *------------------------------------*/
Bool Str_IsAlphaNum(const char* s);

/*--------------------------------------
 * Function: Str_IsNumeric()
 * Parameters:
 *   s  En sträng.
 *
 * Description:
 *   Returnerar sant om strängen endast innehåller numeriska tecken.
 *------------------------------------*/
Bool Str_IsNumeric(const char* s);

/*--------------------------------------
 * Function: Str_Length()
 * Parameters:
 *   s  En sträng.
 *
 * Description:
 *   Returnerar strängens längd i antal tecken.
 *------------------------------------*/
int Str_Length(const char* s);

/*--------------------------------------
 * Function: Str_ToLower()
 * Parameters:
 *   s  En sträng.
 *
 * Description:
 *   Ersätter alla versaler i den angivna strängen med gemener.
 *------------------------------------*/
void Str_ToLower(char* s);

#endif // STRING_H_

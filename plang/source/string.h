/*------------------------------------------------------------------------------
 * File: string.h
 * Created: January 14, 2015
 * Last changed: January 14, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Str�ng-funktioner.
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
 *   Returnerar sant om det angivna tecknet �r alfanumeriskt.
 *------------------------------------*/
Bool Chr_IsAlphaNum(char c);

/*--------------------------------------
 * Function: Chr_IsAlpha()
 * Parameters:
 *   c  Ett tecken.
 *
 * Description:
 *   Returnerar sant om det angivna tecknet �r alfabetiskt.
 *------------------------------------*/
Bool Chr_IsAlpha(char c);

/*--------------------------------------
 * Function: Chr_IsDigit()
 * Parameters:
 *   c  Ett tecken.
 *
 * Description:
 *   Returnerar sant om det angivna tecknet �r numeriskt.
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
 *   s1  Str�ng ett.
 *   s2  Str�ng tv�.
 *
 * Description:
 *   J�mf�r de angivna str�ngarna och returnerar noll om de �r identiska.
 *------------------------------------*/
int Str_Compare(const char* s1, const char* s2);

/*--------------------------------------
 * Function: Str_CompareI()
 * Parameters:
 *   s1  Str�ng ett.
 *   s2  Str�ng tv�.
 *
 * Description:
 *   J�mf�r de angivna str�ngarna utan att ta h�nsyn till gemener/versaler och
 *   returnerar noll om de i �vrigt �r identiska.
 *------------------------------------*/
int Str_CompareI(const char* s1, const char* s2);

/*--------------------------------------
 * Function: Str_Duplicate()
 * Parameters:
 *   s  En str�ng.
 *
 * Description:
 *   Duplicerar den angivna str�ngen i minnet. Gl�m inte anropa free()!
 *------------------------------------*/
char* Str_Duplicate(const char* s);

/*--------------------------------------
 * Function: Str_IsAlpha()
 * Parameters:
 *   s  En str�ng.
 *
 * Description:
 *   Returnerar sant om str�ngen endast inneh�ller alfabetiska tecken.
 *------------------------------------*/
Bool Str_IsAlpha(const char* s);

/*--------------------------------------
 * Function: Str_IsAlphaNum()
 * Parameters:
 *   s  En str�ng.
 *
 * Description:
 *   Returnerar sant om str�ngen endast inneh�ller alfanumeriska tecken.
 *------------------------------------*/
Bool Str_IsAlphaNum(const char* s);

/*--------------------------------------
 * Function: Str_IsNumeric()
 * Parameters:
 *   s  En str�ng.
 *
 * Description:
 *   Returnerar sant om str�ngen endast inneh�ller numeriska tecken.
 *------------------------------------*/
Bool Str_IsNumeric(const char* s);

/*--------------------------------------
 * Function: Str_Length()
 * Parameters:
 *   s  En str�ng.
 *
 * Description:
 *   Returnerar str�ngens l�ngd i antal tecken.
 *------------------------------------*/
int Str_Length(const char* s);

/*--------------------------------------
 * Function: Str_ToLower()
 * Parameters:
 *   s  En str�ng.
 *
 * Description:
 *   Ers�tter alla versaler i den angivna str�ngen med gemener.
 *------------------------------------*/
void Str_ToLower(char* s);

#endif // STRING_H_

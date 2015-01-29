/*------------------------------------------------------------------------------
 * File: string.c
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

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "debug.h"
#include "string.h"

#include <stdlib.h>

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
Bool Chr_IsAlphaNum(char c) {
    return Chr_IsAlpha(c) || Chr_IsDigit(c);
}

/*--------------------------------------
 * Function: Chr_IsAlpha()
 * Parameters:
 *   c  Ett tecken.
 *
 * Description:
 *   Returnerar sant om det angivna tecknet �r alfabetiskt.
 *------------------------------------*/
Bool Chr_IsAlpha(char c) {
    switch (c) {
    case 'A': case 'a':
    case 'B': case 'b':
    case 'C': case 'c':
    case 'D': case 'd':
    case 'E': case 'e':
    case 'F': case 'f':
    case 'G': case 'g':
    case 'H': case 'h':
    case 'I': case 'i':
    case 'J': case 'j':
    case 'K': case 'k':
    case 'L': case 'l':
    case 'M': case 'm':
    case 'N': case 'n':
    case 'O': case 'o':
    case 'P': case 'p':
    case 'Q': case 'q':
    case 'R': case 'r':
    case 'S': case 's':
    case 'T': case 't':
    case 'U': case 'u':
    case 'V': case 'v':
    case 'W': case 'w':
    case 'X': case 'x':
    case 'Y': case 'y':
    case 'Z': case 'z':
        return TRUE;
    }

    return FALSE;
}

/*--------------------------------------
 * Function: Chr_IsDigit()
 * Parameters:
 *   c  Ett tecken.
 *
 * Description:
 *   Returnerar sant om det angivna tecknet �r numeriskt.
 *------------------------------------*/
Bool Chr_IsDigit(char c) {
    switch (c) {
    case '0': case '1': case '2': case '3': case '4': case '5': case '6':
    case '7': case '8': case '9':
        return TRUE;
    }

    return FALSE;
}

/*--------------------------------------
 * Function: Chr_ToLower()
 * Parameters:
 *   c  Ett tecken.
 *
 * Description:
 *   Returnerar sant om det angivna tecknet som gemen.
 *------------------------------------*/
char Chr_ToLower(char c) {
    switch (c) {
    case 'A': return 'a';
    case 'B': return 'b';
    case 'C': return 'c';
    case 'D': return 'd';
    case 'E': return 'e';
    case 'F': return 'f';
    case 'G': return 'g';
    case 'H': return 'h';
    case 'I': return 'i';
    case 'J': return 'j';
    case 'K': return 'k';
    case 'L': return 'l';
    case 'M': return 'm';
    case 'N': return 'n';
    case 'O': return 'o';
    case 'P': return 'p';
    case 'Q': return 'q';
    case 'R': return 'r';
    case 'S': return 's';
    case 'T': return 't';
    case 'U': return 'u';
    case 'V': return 'v';
    case 'W': return 'w';
    case 'X': return 'x';
    case 'Y': return 'y';
    case 'Z': return 'z';

    case '�': return '�';
    case '�': return '�';
    case '�': return '�';
    case '�': return '�';
    case '�': return '�';
    case '�': return '�';
    case '�': return '�';
    case '�': return '�';
    case '�': return '�';
    case '�': return '�';
    case '�': return '�';

    case '�': return '�';
    case '�': return '�';
    case '�': return '�';
    case '�': return '�';
    case '�': return '�';

    case '�': return '�';
    case '�': return '�';
    case '�': return '�';
    case '�': return '�';
    case '�': return '�';

    case '�': return '�';
    case '�': return '�';

    case '�': return '�';
    }

    return c;
}

/*--------------------------------------
 * Function: Str_Compare()
 * Parameters:
 *   s1  Str�ng ett.
 *   s2  Str�ng tv�.
 *
 * Description:
 *   J�mf�r de angivna str�ngarna och returnerar noll om de �r identiska.
 *------------------------------------*/
int Str_Compare(const char* s1, const char* s2) {
    if (s1 == s2)
        return 0;

    while (*s1 && *s1==*s2) {
        s1++;
        s2++;
    }

    return *s1 - *s2;
}

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
int Str_CompareI(const char* s1, const char* s2) {
    if (s1 == s2)
        return 0;

    do {
        int diff = Chr_ToLower(*s1) - Chr_ToLower(*s2);
        if (diff != 0)
            return diff;
        s1++;
        s2++;
    } while (*s1);

    return 0;
}

/*--------------------------------------
 * Function: Str_Duplicate()
 * Parameters:
 *   s  En str�ng.
 *
 * Description:
 *   Duplicerar den angivna str�ngen i minnet. Gl�m inte anropa free()!
 *------------------------------------*/
char* Str_Duplicate(const char* s) {
    int   len = Str_Length(s);
    char* s2  = malloc((size_t)len+1);

    for (int i = 0; i <= len; i++)
        s2[i] = s[i];

    return s2;
}

/*--------------------------------------
 * Function: Str_IsAlpha()
 * Parameters:
 *   s  En str�ng.
 *
 * Description:
 *   Returnerar sant om str�ngen endast inneh�ller alfabetiska tecken.
 *------------------------------------*/
Bool Str_IsAlpha(const char* s) {
    while (*s) {
        if (!Chr_IsAlpha(*(s++)))
            return FALSE;
    }

    return TRUE;
}

/*--------------------------------------
 * Function: Str_IsAlphaNum()
 * Parameters:
 *   s  En str�ng.
 *
 * Description:
 *   Returnerar sant om str�ngen endast inneh�ller alfanumeriska tecken.
 *------------------------------------*/
Bool Str_IsAlphaNum(const char* s) {
    while (*s) {
        if (!Chr_IsAlphaNum(*(s++)))
            return FALSE;
    }

    return TRUE;
}

/*--------------------------------------
 * Function: Str_IsNumeric()
 * Parameters:
 *   s  En str�ng.
 *
 * Description:
 *   Returnerar sant om str�ngen endast inneh�ller numeriska tecken.
 *------------------------------------*/
Bool Str_IsNumeric(const char* s) {
    while (*s) {
        if (!Chr_IsDigit(*(s++)))
            return FALSE;
    }

    return TRUE;
}

/*--------------------------------------
 * Function: Str_Length()
 * Parameters:
 *   s  En str�ng.
 *
 * Description:
 *   Returnerar str�ngens l�ngd i antal tecken.
 *------------------------------------*/
int Str_Length(const char* s) {
    int i = 0;
    while (*(s++))
        i++;

    return i;
}

/*--------------------------------------
 * Function: Str_ToLower()
 * Parameters:
 *   s  En str�ng.
 *
 * Description:
 *   Ers�tter alla versaler i den angivna str�ngen med gemener.
 *------------------------------------*/
void Str_ToLower(char* s) {
    while (*s) {
        *s = Chr_ToLower(*s);
        s++;
    }
}

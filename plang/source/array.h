/*------------------------------------------------------------------------------
 * File: array.h
 * Created: January 2, 2015
 * Last changed: January 9, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder en dynamisk array som v�xer automatiskt med antalet objekt som
 *   l�ggs in.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifndef ARRAY_H_
#define ARRAY_H_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "common.h"
#include "debug.h"

#include <stdlib.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: Array
 *
 * Description:
 *   Representerar en dynamisk array med objekt i.
 *------------------------------------*/
typedef struct {
    void*  elems;
    int    num_elems;
    int    max_elems;
    size_t elem_size;
} Array;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: Array_AddElem()
 * Parameters:
 *   array  Den array till vilken vi ska l�gga ett element.
 *   elem   Elementet som ska l�ggas till i arrayen.
 *
 * Description:
 *   L�gger till ett element i en array. Returnerar minnesadressen d�r noden
 *   lades in.
 *------------------------------------*/
void* Array_AddElem(Array* array, const void* elem);

/*--------------------------------------
 * Function: Array_Free()
 * Parameters:
 *   array  Den array till vilken vi ska l�gga ett element.
 *
 * Description:
 *   Sl�pper en array ur minnet.
 *------------------------------------*/
void Array_Free(Array* array);

/*--------------------------------------
 * Function: Array_GetElemPtr()
 * Parameters:
 *   array  Den array fr�n vilken vi ska l�sa ett element.
 *   i      Det index i arrayen fr�n vilket vi ska l�sa elementet.
 *
 * Description:
 *   L�ser ut och returnerar en pekare till det specificerade elementet i
 *   arrayen.
 *------------------------------------*/
INLINE_HINT
void* Array_GetElemPtr(const Array* array, int i) {
    ASSERT(0 <= i && i < array->num_elems)
    return (char*)array->elems + (i * array->elem_size);
}

/*--------------------------------------
 * Function: Array_Init()
 * Parameters:
 *   array  Den array som ska initieras.
 *
 * Description:
 *   Initialiserar och allokerar en array.
 *------------------------------------*/
void Array_Init(Array* array, size_t elem_size);

/*--------------------------------------
 * Function: Array_Length()
 * Parameters:
 *   array  Arrayen vars l�ngd ska l�sas ut.
 *
 * Description:
 *   Returnerar den specificerade arrayens l�ngd.
 *------------------------------------*/
INLINE_HINT
int Array_Length(const Array* array) {
    return array->num_elems;
}

#endif // ARRAY_H_

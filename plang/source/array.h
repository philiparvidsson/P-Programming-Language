/*------------------------------------------------------------------------------
 * File: array.h
 * Created: January 2, 2015
 * Last changed: January 9, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder en dynamisk array som växer automatiskt med antalet objekt som
 *   läggs in.
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
 *   array  Den array till vilken vi ska lägga ett element.
 *   elem   Elementet som ska läggas till i arrayen.
 *
 * Description:
 *   Lägger till ett element i en array. Returnerar minnesadressen där noden
 *   lades in.
 *------------------------------------*/
void* Array_AddElem(Array* array, const void* elem);

/*--------------------------------------
 * Function: Array_Free()
 * Parameters:
 *   array  Den array till vilken vi ska lägga ett element.
 *
 * Description:
 *   Släpper en array ur minnet.
 *------------------------------------*/
void Array_Free(Array* array);

/*--------------------------------------
 * Function: Array_GetElemPtr()
 * Parameters:
 *   array  Den array från vilken vi ska läsa ett element.
 *   i      Det index i arrayen från vilket vi ska läsa elementet.
 *
 * Description:
 *   Läser ut och returnerar en pekare till det specificerade elementet i
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
 *   array  Arrayen vars längd ska läsas ut.
 *
 * Description:
 *   Returnerar den specificerade arrayens längd.
 *------------------------------------*/
INLINE_HINT
int Array_Length(const Array* array) {
    return array->num_elems;
}

#endif // ARRAY_H_

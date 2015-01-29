/*------------------------------------------------------------------------------
 * File: array.c
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

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "array.h"

#include <stdlib.h>
#include <string.h> // memcpy()

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: INITIAL_MAX_ELEMS
 *
 * Description:
 *   Det initiala antalet element som minne ska allokeras f�r i Array_Init().
 *------------------------------------*/
#define INITIAL_MAX_ELEMS 8 // 8 �r nog lagom.

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
void* Array_AddElem(Array* array, const void* elem) {
    if (array->num_elems >= array->max_elems) {
        // Arrayen �r full, s� vi dubblar kapaciteten och kopierar �ver de gamla
        // elementen till den nya minnesplatsen, sen sl�pper vi den gamla
        // arrayen ur minnet.

        void* old_elems     = array->elems;
        int   old_max_elems = array->max_elems;

        array->max_elems *= 2;
        array->elems = malloc(array->max_elems * array->elem_size);
        memcpy(array->elems, old_elems, old_max_elems * array->elem_size);
        free(old_elems);
    }

    void* dest = (char*)array->elems + (array->num_elems * array->elem_size);
    memcpy(dest, elem, array->elem_size);

    array->num_elems++;
    return dest;
}

/*--------------------------------------
 * Function: Array_Free()
 * Parameters:
 *   array  Den array till vilken vi ska l�gga ett element.
 *
 * Description:
 *   Sl�pper en array ur minnet.
 *------------------------------------*/
void Array_Free(Array* array) {
    ASSERT(array->elems != NULL);

    free(array->elems);

    array->elems     = NULL;
    array->num_elems = 0;
    array->max_elems = 0;
    array->elem_size = 0;
}

/*--------------------------------------
 * Function: Array_Init()
 * Parameters:
 *   array  Den array som ska initieras.
 *
 * Description:
 *   Initialiserar och allokerar en array.
 *------------------------------------*/
void Array_Init(Array* array, size_t elem_size) {
    array->num_elems = 0;
    array->max_elems = INITIAL_MAX_ELEMS;
    array->elem_size = elem_size;

    array->elems = malloc(array->max_elems * array->elem_size);
}

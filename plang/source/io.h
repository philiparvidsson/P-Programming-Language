/*------------------------------------------------------------------------------
 * File: io.h
 * Created: January 4, 2015
 * Last changed: January 9, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Funktioner för utskrivning och inläsning av data till och från användaren
 *   eller filer.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifndef IO_H_
#define IO_H_

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: IO_GetIntFromUser()
 * Parameters:
 *
 * Description:
 *   Låter användaren skriva in ett heltal.
 *------------------------------------*/
int IO_GetIntFromUser();

/*--------------------------------------
 * Function: IO_GetStrFromUser()
 * Parameters:
 *
 * Description:
 *   Låter användaren skriva in en sträng. Glöm inte anropa free() efteråt för
 *   att förhindra minnesläckage.
 *------------------------------------*/
char* IO_GetStrFromUser();

/*--------------------------------------
 * Function: IO_Pause()
 * Parameters:
 *
 * Description:
 *   Pausar tills användaren trycker enter.
 *------------------------------------*/
void IO_Pause();

/*--------------------------------------
 * Function: IO_ReadFile()
 * Parameters:
 *   file_name  Namnet på filen som ska läsas in.
 *
 * Description:
 *   Läser in en fil och returnerar innehållet som en sträng. Glöm inte anropa
 *   free()!
 *------------------------------------*/
char* IO_ReadFile(const char* file_name);

#endif // IO_H_

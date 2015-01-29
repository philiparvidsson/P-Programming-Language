/*------------------------------------------------------------------------------
 * File: io.h
 * Created: January 4, 2015
 * Last changed: January 9, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Funktioner f�r utskrivning och inl�sning av data till och fr�n anv�ndaren
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
 *   L�ter anv�ndaren skriva in ett heltal.
 *------------------------------------*/
int IO_GetIntFromUser();

/*--------------------------------------
 * Function: IO_GetStrFromUser()
 * Parameters:
 *
 * Description:
 *   L�ter anv�ndaren skriva in en str�ng. Gl�m inte anropa free() efter�t f�r
 *   att f�rhindra minnesl�ckage.
 *------------------------------------*/
char* IO_GetStrFromUser();

/*--------------------------------------
 * Function: IO_Pause()
 * Parameters:
 *
 * Description:
 *   Pausar tills anv�ndaren trycker enter.
 *------------------------------------*/
void IO_Pause();

/*--------------------------------------
 * Function: IO_ReadFile()
 * Parameters:
 *   file_name  Namnet p� filen som ska l�sas in.
 *
 * Description:
 *   L�ser in en fil och returnerar inneh�llet som en str�ng. Gl�m inte anropa
 *   free()!
 *------------------------------------*/
char* IO_ReadFile(const char* file_name);

#endif // IO_H_

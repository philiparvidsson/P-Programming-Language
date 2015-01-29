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
 *   * Ändrade så IO_GetIntFromUser() inte accepterar tomma inputs.
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "common.h"
#include "io.h"
#include "string.h"

#include <stdio.h>
#include <stdlib.h>

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
int IO_GetIntFromUser() {
    char buf[16];
    Bool is_valid_int = FALSE;

    while (!is_valid_int) {
        fgets(buf, sizeof(buf), stdin);

        int len = Str_Length(buf);

        is_valid_int = TRUE;
        for (int i = 0; i < len; i++) {
            char c = buf[i];

            if (c=='\r' || c=='\n') {
                buf[i] = '\0';
                break;
            }

            if (!Chr_IsDigit(c)) {
                is_valid_int = FALSE;
                break;
            }
        }

        if (Str_Length(buf) == 0)
            is_valid_int = FALSE;

        if (!is_valid_int)
            printf("Invalid integer. Try again: ");
    }

    return atoi(buf);
}

/*--------------------------------------
 * Function: IO_GetStrFromUser()
 * Parameters:
 *
 * Description:
 *   Låter användaren skriva in en sträng. Glöm inte anropa free() efteråt för
 *   att förhindra minnesläckage.
 *------------------------------------*/
char* IO_GetStrFromUser() {
    char buf[1024];

    fgets(buf, sizeof(buf), stdin);

    int len = Str_Length(buf);
    for (int i = 0; i < len; i++) {
        char c = buf[i];
        if (c == '\r' || c == '\n') {
            buf[i] = 0;
            break;
        }
    }

    return Str_Duplicate(buf);
}

/*--------------------------------------
 * Function: IO_Pause()
 * Parameters:
 *
 * Description:
 *   Pausar tills användaren trycker enter.
 *------------------------------------*/
void IO_Pause() {
    printf("\nPress ENTER to continue...\n");
    getchar();
}

/*--------------------------------------
 * Function: IO_ReadFile()
 * Parameters:
 *   file_name  Namnet på filen som ska läsas in.
 *
 * Description:
 *   Läser in en fil och returnerar innehållet som en sträng. Glöm inte anropa
 *   free()!
 *------------------------------------*/
char* IO_ReadFile(const char* file_name) {
    FILE* fp = fopen(file_name, "rb");

    if (!fp)
        return NULL;

    fseek(fp, 0, SEEK_END);
    long num_bytes = ftell(fp);
    rewind(fp);

    char* s = malloc(num_bytes+1);
    fread(s, sizeof(char), num_bytes, fp);
    s[num_bytes] = '\0';
    fclose(fp);

    return s;
}

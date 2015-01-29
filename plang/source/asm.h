/*------------------------------------------------------------------------------
 * File: asm.h
 * Created: January 5, 2015
 * Last changed: January 5, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Funktioner för att generera assembly-x86-kod av ett abstrakt syntax-träd
 *   och generera en exe-fil av det. Koden som genereras är något specifik för
 *   flat assembler (fasm).
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifndef ASM_H_
#define ASM_H_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "ast.h"
#include "common.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: Asm_GenerateCode()
 * Parameters:
 *   root       Root-noden i det AST som ska kompileras till assembly-kod.
 *   file_name  Namnet på filen som koden ska skrivas ut till.
 *   optimize   Huruvida den genererade koden ska optimeras eller inte.
 *
 * Description:
 *   Genererar assembly-kod för ett AST och skriver ut det till en fil.
 *------------------------------------*/
Bool Asm_GenerateCode(const AST_Node* root, const char* file_name,
                      Bool optimize);

#endif // ASM_H_

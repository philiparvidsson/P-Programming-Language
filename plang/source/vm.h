/*------------------------------------------------------------------------------
 * File: vm.h
 * Created: January 3, 2015
 * Last changed: January 11, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder funktioner för exekvering av abstrakta syntax-träd i en virtuell
 *   maskin.
 *
 * Changes:
 *   * Lade till typen VM_Config för att möjliggöra konfigurering av den
 *     virtuella maskinen.
 *
 *----------------------------------------------------------------------------*/

#ifndef VM_H_
#define VM_H_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "ast.h"
#include "common.h"

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: VM_ERR_INF_LOOP
 *
 * Description:
 *   Indikerar att exekveringen fastnade i en oändlig loop.
 *------------------------------------*/
#define VM_ERR_INF_LOOP -2

/*--------------------------------------
 * Constant: VM_ERR_INVALID_VAR
 *
 * Description:
 *   Indikerar att en ogiltig variabel användes, för att den låg utanför
 *   variabel-arrayen.
 *------------------------------------*/
#define VM_ERR_INVALID_VAR -3

/*--------------------------------------
 * Constant: VM_ERR_OVERFLOW
 *
 * Description:
 *   Indikerar att ett variabelvärde översvämmades.
 *------------------------------------*/
#define VM_ERR_OVERFLOW -4

/*--------------------------------------
 * Constant: VM_ERR_PREMATURE_RESULT
 *
 * Description:
 *   Indikerar att en RESULT-nod påträffades för tidigt.
 *------------------------------------*/
#define VM_ERR_PREMATURE_RESULT -5

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: VM_Config
 *
 * Description:
 *   Beskriver den virtuella maskinens konfiguration.
 *------------------------------------*/
typedef struct {
    int  vars[PLANG_NUM_VARS];
    Bool enable_debug;
} VM_Config;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: VM_ExecAST()
 * Parameters:
 *   ast     Det abstrakta syntax-träd som ska exekveras.
 *   config  Den virtuella maskinens konfiguration.
 *
 * Description:
 *   Exekverar det specificerade abstrakta syntax-trädet i en virtuell maskin.
 *------------------------------------*/
int VM_ExecAST(AST_Node* ast, VM_Config* config);

/*--------------------------------------
 * Function: VM_StateDump()
 * Parameters:
 *   node    Noden varifrån vi skriver ut källkod och variabelvärden.
 *   indent  Indentering i antal mellanslag.
 *   vm      Den virtuella maskinens konfiguration.
 *
 * Description:
 *   Dumpar den virtuella maskinens nuvarande s.k. state och skriver ut det i
 *   form av källkod och variabelvärden.
 *------------------------------------*/
void VM_StateDump(AST_Node* node, int indent, const VM_Config* vm);

#endif // VM_H_

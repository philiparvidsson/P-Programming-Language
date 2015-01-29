/*------------------------------------------------------------------------------
 * File: vm.c
 * Created: January 3, 2015
 * Last changed: January 11, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder funktioner f�r exekvering av abstrakta syntax-tr�d i en virtuell
 *   maskin.
 *
 * Changes:
 *   * �ndrade s� VM inte avbryter exekvering om PRED k�rs p� en variabel med
 *     v�rdet noll. Ist�llet begr�nsas v�rdet s� att det helt enkelt inte g�r
 *     under noll .
 *   * St�d f�r VM_Config.
 *   * Felkod f�r overflow.
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "array.h"
#include "ast.h"
#include "common.h"
#include "debug.h"
#include "io.h"
#include "vm.h"

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: NO_RESULT
 *
 * Description:
 *   Indikerar att inget resultat finns.
 *------------------------------------*/
#define NO_RESULT -1

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: IsLastNode()
 * Parameters:
 *   node  Noden som ska kontrolleras.
 *
 * Description:
 *   Kontrollerar om den specificerade noden �r den sista i sitt tr�d.
 *------------------------------------*/
static Bool IsLastNode(const AST_Node* node) {
    // Om den angivna noden har n�gra barn kan vi g� en niv� ned�t, varf�r den
    // om�jligt kan vara den sista i tr�det.
    int num_children = Array_Length(&node->children);
    if (num_children > 0)
        return FALSE;

    AST_Node* parent = node->parent;
    if (parent) {
        int       num_siblings = Array_Length(&parent->children);
        AST_Node* last_sibling = Array_GetElemPtr(&parent->children, num_siblings-1);

        // Om den specificerade noden inte �r den sista i syskonskaran s� �r det
        // inte heller den sista noden i sitt tr�d.
        if (node != last_sibling)
            return FALSE;
    }

    return TRUE;
}

/*--------------------------------------
 * Function: ExecNode()
 * Parameters:
 *   node    Den nod som ska k�ras.
 *   vm      Den virtuella maskinens konfiguration.
 *   result  Pekare till den int som resultatet till slut ska sparas i.
 *
 * Description:
 *   Exekverar den specificerade AST-noden.
 *------------------------------------*/
static void ExecNode(AST_Node* node, VM_Config* vm, int* result) {
    // Om ett resultat lagrats avbryter vi exekveringen.
    if (*result != NO_RESULT)
        return;

    switch (node->type) {
    /*----------------------------------------------------
     * PROGRAM (<variabel>[, <variabel>])
     *--------------------------------------------------*/
    case AST_PROGRAM: {
        // En programnod exekverar vi genom att rekursivt exekvera alla barn-
        // noder.

        int num_children = Array_Length(&node->children);
        for (int i = 0; i < num_children; i++) {
            if (*result != NO_RESULT)
                return;

            AST_Node* child = Array_GetElemPtr(&node->children, i);
            ExecNode(child, vm, result);
        }

        break;
    }

    /*----------------------------------------------------
     * <variabel> := <naturligt-tal>
     *--------------------------------------------------*/
    case AST_ASSIGN: {
        // Tilldelning av en variabel, s� vi g�r motsvarande �ndring i vars-
        // arrayen.

        int var = *(int*)Array_GetElemPtr(&node->values, 0);
        if (var < 0 || var >= PLANG_NUM_VARS) {
            *result = VM_ERR_INVALID_VAR;
            return;
        }

        int val = *(int*)Array_GetElemPtr(&node->values, 1);
        if (val < 0)
            val = 0;

        vm->vars[var] = val;
        break;
    }

    /*----------------------------------------------------
     * <variabel> := PRED(<variabel>)
     * <variabel> := SUCC(<variabel>)
     *--------------------------------------------------*/
    case AST_PRED:
    case AST_SUCC: {
        // Antingen PRED eller SUCC. De b�da noderna exekveras p� exakt samma
        // s�tt, f�rutom att PRED subtraherar 1, och SUCC adderar 1. �ndringar
        // g�r vi i vars-arrayen.

        int var0 = *(int*)Array_GetElemPtr(&node->values, 0);
        int var1 = *(int*)Array_GetElemPtr(&node->values, 1);
        if (var0 < 0 || var0 >= PLANG_NUM_VARS
         || var1 < 0 || var1 >= PLANG_NUM_VARS)
        {
            *result = VM_ERR_INVALID_VAR;
            return;
        }

        int* vars = vm->vars; // Lokal pekare f�r snabbare minnesaccess.

        if (var0 == var1) {
            if (node->type == AST_PRED) vars[var0]--; // PRED
            else                        vars[var0]++; // SUCC
        }
        else {
            if (node->type == AST_PRED) vars[var0] = vars[var1] - 1; // PRED
            else                        vars[var0] = vars[var1] + 1; // SUCC
        }

        if (vars[var0] < 0) {
            // Negativa v�rden �r inte till�tna (ifall PRED k�rs p� en variabel
            // med v�rdet noll).

            if (node->type == AST_SUCC) {
                *result = VM_ERR_OVERFLOW;
                return;
            }

            vars[var0] = 0;
        }

        break;
    }

    /*----------------------------------------------------
     * WHILE <variabel> != 0 DO ... END
     *--------------------------------------------------*/
    case AST_WHILE: {
        // While-loopar exekverar vi genom att k�ra alla barn-noder om och om
        // igen tills variabeln i loop-villkoret n�r v�rdet noll.

        int var = *(int*)Array_GetElemPtr(&node->values, 0);
        if (var < 0 || var >= PLANG_NUM_VARS) {
            *result = VM_ERR_INVALID_VAR;
            return;
        }

        if (vm->enable_debug) {
            VM_StateDump(AST_FindRoot(node), 0, vm);
            IO_Pause();
        }

        int val = vm->vars[var]; // Vi anv�nder val f�r att motverka o�ndliga
                                 // loopar.

        int num_children = Array_Length(&node->children);
        while (vm->vars[var]) {
            for (int i = 0; i < num_children; i++) {
                if (*result != NO_RESULT)
                    return;

                AST_Node* child = Array_GetElemPtr(&node->children, i);
                ExecNode(child, vm, result);
            }

            if (vm->vars[var] == val) {
                // Variabeln i loop-villkoret har inte f�r�ndats efter en hel
                // iteration, vilket m�ste inneb�ra att den h�r loopen �r
                // o�ndlig. Vi kan likv�l hoppa hur den h�r, annars kommer
                // programmet att h�nga sig.
                // TODO: Den h�r kontrollen g�r inte att lita p�, hitta p� en
                //         b�ttre.
                //*result = VM_ERR_INF_LOOP;
                //return;
            }

            if (vm->enable_debug) {
                // I debug-l�ge pausar vi loopen och skriver ut k�llkod samt
                // variabelv�rden.
                VM_StateDump(AST_FindRoot(node), 0, vm);
                IO_Pause();
            }
        }

        break;
    }

    /*----------------------------------------------------
     * RESULT (<variabel>)
     *--------------------------------------------------*/
    case AST_RESULT: {
        // Result-noden exekveras enkelt genom att spara den aktuella variabeln
        // i result-parametern.

        // TODO: Unders�k om det �r l�mpligare att spara index till variabeln.
        int var = *(int*)Array_GetElemPtr(&node->values, 0);
        if (var < 0 || var >= PLANG_NUM_VARS) {
            *result = VM_ERR_INVALID_VAR;
            return;
        }

        *result = IsLastNode(node) ? vm->vars[var]
                                   : VM_ERR_PREMATURE_RESULT;

        break;
    }

    default:
        // Det h�r ska inte h�nda.
        FAIL();
    }
}

/*--------------------------------------
 * Function: VM_ExecAST()
 * Parameters:
 *   ast     Det abstrakta syntax-tr�d som ska exekveras.
 *   config  Den virtuella maskinens konfiguration.
 *
 * Description:
 *   Exekverar det specificerade abstrakta syntax-tr�det i en virtuell maskin.
 *------------------------------------*/
int VM_ExecAST(AST_Node* ast, VM_Config* conf) {
    ASSERT(ast->type == AST_PROGRAM);

    int result = NO_RESULT;

    ExecNode(ast, conf, &result);

    return result;
}

/*--------------------------------------
 * Function: VM_StateDump()
 * Parameters:
 *   node    Noden varifr�n vi skriver ut k�llkod och variabelv�rden.
 *   indent  Indentering i antal mellanslag.
 *   vm      Den virtuella maskinens konfiguration.
 *
 * Description:
 *   Dumpar den virtuella maskinens nuvarande s.k. state och skriver ut det i
 *   form av k�llkod och variabelv�rden.
 *------------------------------------*/
void VM_StateDump(AST_Node* node, int indent, const VM_Config* vm) {
    if (node->type != AST_RESULT) {
        for (int i = 0; i < indent; i++)
            printf(" ");
    }

    switch (node->type) {
    case AST_ASSIGN: {
        int var = *(int*)Array_GetElemPtr(&node->values, 0);
        int val = *(int*)Array_GetElemPtr(&node->values, 1);
        printf("X%d := %d # %d\n", var, val, vm->vars[var]);
        break;
    }

    case AST_PRED: {
        int var0 = *(int*)Array_GetElemPtr(&node->values, 0);
        int var1 = *(int*)Array_GetElemPtr(&node->values, 1);
        printf("X%d := PRED(X%d) # %d\n", var0, var1, vm->vars[var0]);
        break;
    }

    case AST_PROGRAM: {
        printf("PROGRAM (");

        int num_inputs = Array_Length(&node->values);
        for (int i = 0; i < num_inputs; i++) {
            int var = *(int*)Array_GetElemPtr(&node->values, i);
            if (i > 0)
                printf(", ");
            printf("X%d", var);
        }

        printf(")\n");

        int num_children = Array_Length(&node->children);
        for (int i = 0; i < num_children; i++) {
            AST_Node* child = Array_GetElemPtr(&node->children, i);
            VM_StateDump(child, indent+4, vm);
        }

        break;
    }

    case AST_RESULT: {
        int var = *(int*)Array_GetElemPtr(&node->values, 0);
        printf("RESULT (X%d)\n", var);
        break;
    }

    case AST_SUCC: {
        int var0 = *(int*)Array_GetElemPtr(&node->values, 0);
        int var1 = *(int*)Array_GetElemPtr(&node->values, 1);
        printf("X%d := SUCC(X%d) # %d\n", var0, var1, vm->vars[var0]);
        break;
    }

    case AST_WHILE: {
        int var = *(int*)Array_GetElemPtr(&node->values, 0);

        printf("WHILE X%d != 0 DO\n", var);

        int num_children = Array_Length(&node->children);
        for (int i = 0; i < num_children; i++) {
            AST_Node* child = Array_GetElemPtr(&node->children, i);
            VM_StateDump(child, indent+4, vm);
        }

        for (int i = 0; i < indent; i++)
            printf(" ");

        printf("END\n");

        break;
    }

    default:
        FAIL();
    }
}

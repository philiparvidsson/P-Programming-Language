/*------------------------------------------------------------------------------
 * File: ast.h
 * Created: January 3, 2015
 * Last changed: January 13, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Den h�r modulen erbjuder funktionalitet f�r att bygga ett abstrakt syntax-
 *   tr�d.
 *
 * Changes:
 *   Lade till parent-f�ltet i AST_Node-structen.
 *
 *----------------------------------------------------------------------------*/

#ifndef AST_H_
#define AST_H_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "array.h"
#include "common.h"
#include "tokenizer.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: AST_Node_Type
 *
 * Description:
 *   Den h�r enum-typen beskriver de olika slags AST-noder som finns.
 *------------------------------------*/
typedef enum {
    AST_ASSIGN,
    AST_PRED,
    AST_PROGRAM,
    AST_RESULT,
    AST_SUCC,
    AST_WHILE
} AST_Node_Type;

/*--------------------------------------
 * Type: AST_Node
 *
 * Description:
 *   Den h�r typen representerar en enskild nod i ett abstrakt syntax-tr�d.
 *------------------------------------*/
typedef struct AST_Node {
           AST_Node_Type type;
    struct AST_Node*     parent;
           Array         children;
           Array         values;
} AST_Node;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: AST_AddChild()
 * Parameters:
 *   parent  Den nod vilken vi ska l�gga till ett "barn" till.
 *   child   Noden som ska l�ggas till som "barn" till f�r�ldranoden.
 *
 * Description:
 *   Den h�r funktionen l�gger till en nod som barn till en annan nod.
 *------------------------------------*/
INLINE_HINT
void* AST_AddChild(AST_Node* parent, AST_Node* child) {
    ASSERT(child->parent == NULL);

    child->parent = parent;
    return Array_AddElem(&parent->children, child);
}

/*--------------------------------------
 * Function: AST_AddValue()
 * Parameters:
 *   node   Den nod till vilken vi ska l�gga ett v�rde.
 *   value  V�rdet som ska l�ggas till.
 *
 * Description:
 *   Den h�r funktionen l�gger ett v�rde till en nod.
 *------------------------------------*/
INLINE_HINT
void AST_AddValue(AST_Node* node, int value) {
    Array_AddElem(&node->values, &value);
}

/*--------------------------------------
 * Function: AST_CreateNode()
 * Parameters:
 *   type  Typen av noden som ska skapas.
 *
 * Description:
 *   Den h�r funktionen skapar en AST-nod.
 *------------------------------------*/
AST_Node AST_CreateNode(AST_Node_Type type);

/*--------------------------------------
 * Function: AST_FindRoot()
 * Parameters:
 *   node  En nod i det tr�d vars rot s�kes.
 *
 * Description:
 *   Returnerar root-noden i det givna tr�det.
 *------------------------------------*/
INLINE_HINT
AST_Node* AST_FindRoot(AST_Node* node) {
    while (node->parent != NULL)
        node = node->parent;

    // Root-noden m�ste alltid vara en PROGRAM-nod.
    ASSERT(node->type == AST_PROGRAM);

    return node;
}

/*--------------------------------------
 * Function: AST_Repair()
 * Parameters:
 *   node  Noden som ska fixas.
 *
 * Description:
 *   Reparerar den specificerade noden genom att �terst�lla alla f�r�ldra-
 *   l�nkar.
 *------------------------------------*/
void AST_Repair(AST_Node* node);

/*--------------------------------------
 * Function: AST_GenerateTree()
 * Parameters:
 *   tokens  En array inneh�llande s.k. tokens, f�rslagsvis genererade av
 *           funktionen Tok_Tokenize() i modulen tokenizer.c.
 *
 * Description:
 *   Den h�r funktionen genererar ett abstrakt syntax-tr�d fr�n en array av
 *   tokens.
 *------------------------------------*/
AST_Node AST_GenerateTree(const Array* tokens);

/*--------------------------------------
 * Function: AST_PrintNode()
 * Parameters:
 *   node    Noden varifr�n tr�det ska skrivas ut.
 *   indent  Indentering i antal mellanslag.
 *
 * Description:
 *   Skriver ut det abstrakta syntax-tr�det.
 *------------------------------------*/
void AST_PrintNode(const AST_Node* node, int indent);

#endif // AST_H_

/*------------------------------------------------------------------------------
 * File: ast.h
 * Created: January 3, 2015
 * Last changed: January 13, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Den här modulen erbjuder funktionalitet för att bygga ett abstrakt syntax-
 *   träd.
 *
 * Changes:
 *   Lade till parent-fältet i AST_Node-structen.
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
 *   Den här enum-typen beskriver de olika slags AST-noder som finns.
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
 *   Den här typen representerar en enskild nod i ett abstrakt syntax-träd.
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
 *   parent  Den nod vilken vi ska lägga till ett "barn" till.
 *   child   Noden som ska läggas till som "barn" till föräldranoden.
 *
 * Description:
 *   Den här funktionen lägger till en nod som barn till en annan nod.
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
 *   node   Den nod till vilken vi ska lägga ett värde.
 *   value  Värdet som ska läggas till.
 *
 * Description:
 *   Den här funktionen lägger ett värde till en nod.
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
 *   Den här funktionen skapar en AST-nod.
 *------------------------------------*/
AST_Node AST_CreateNode(AST_Node_Type type);

/*--------------------------------------
 * Function: AST_FindRoot()
 * Parameters:
 *   node  En nod i det träd vars rot sökes.
 *
 * Description:
 *   Returnerar root-noden i det givna trädet.
 *------------------------------------*/
INLINE_HINT
AST_Node* AST_FindRoot(AST_Node* node) {
    while (node->parent != NULL)
        node = node->parent;

    // Root-noden måste alltid vara en PROGRAM-nod.
    ASSERT(node->type == AST_PROGRAM);

    return node;
}

/*--------------------------------------
 * Function: AST_Repair()
 * Parameters:
 *   node  Noden som ska fixas.
 *
 * Description:
 *   Reparerar den specificerade noden genom att återställa alla föräldra-
 *   länkar.
 *------------------------------------*/
void AST_Repair(AST_Node* node);

/*--------------------------------------
 * Function: AST_GenerateTree()
 * Parameters:
 *   tokens  En array innehållande s.k. tokens, förslagsvis genererade av
 *           funktionen Tok_Tokenize() i modulen tokenizer.c.
 *
 * Description:
 *   Den här funktionen genererar ett abstrakt syntax-träd från en array av
 *   tokens.
 *------------------------------------*/
AST_Node AST_GenerateTree(const Array* tokens);

/*--------------------------------------
 * Function: AST_PrintNode()
 * Parameters:
 *   node    Noden varifrån trädet ska skrivas ut.
 *   indent  Indentering i antal mellanslag.
 *
 * Description:
 *   Skriver ut det abstrakta syntax-trädet.
 *------------------------------------*/
void AST_PrintNode(const AST_Node* node, int indent);

#endif // AST_H_

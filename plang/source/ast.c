/*------------------------------------------------------------------------------
 * File: ast.c
 * Created: January 2, 2015
 * Last changed: January 12, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   This file provides function for managing an abstract syntax tree.
 *
 * Changes:
 *   * Tilldelning av andra värden än noll är nu möjligt.
 *   * Tilldelar parent ett värde.
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "array.h"
#include "ast.h"
#include "common.h"
#include "debug.h"
#include "tokenizer.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: ParseTokens()
 * Parameters:
 *   node    Noden till vilken barn-noder ska läggas.
 *   tokens  En array som innehåller de tokens som ska läsas av.
 *   i       Det index i token-arrayen som avläsningen ska börja på.
 *
 * Description:
 *   Läser av tokens och bygger ett AST.
 *------------------------------------*/
static int ParseTokens(AST_Node* node, const Array* tokens, int i) {
    while (TRUE) {
        P_Token* tok = Array_GetElemPtr(tokens, i++);

        switch (tok->type) {
        /*----------------------------------------------------
         * <variabel> := <naturligt-tal>
         * <variabel> := PRED(<variabel>)
         * <variabel> := SUCC(<variabel>)
         *--------------------------------------------------*/
        case PTOK_IDENT: {
            P_Token* assign_tok        = Array_GetElemPtr(tokens, i++);
            P_Token* int_pred_succ_tok = Array_GetElemPtr(tokens, i++);

            ASSERT(assign_tok->type == PTOK_ASSIGN);

            if (int_pred_succ_tok->type == PTOK_INT) {
                // <variabel> := <naturligt-tal>

                AST_Node  assign_node     = AST_CreateNode(AST_ASSIGN);
                AST_Node* assign_node_ptr = AST_AddChild(node, &assign_node);

                // Vi lägger in variabelindex och tilldelningsvärde.
                AST_AddValue(assign_node_ptr, atoi(tok->value+1));
                AST_AddValue(assign_node_ptr, atoi(int_pred_succ_tok->value));
            }
            else if (int_pred_succ_tok->type == PTOK_PRED
                  || int_pred_succ_tok->type == PTOK_SUCC)
            {
                // <variabel> := PRED(<variabel>)
                // <variabel> := SUCC(<variabel>)

                P_Token* lparen_tok = Array_GetElemPtr(tokens, i++);
                P_Token* ident_tok  = Array_GetElemPtr(tokens, i++);
                P_Token* rparen_tok = Array_GetElemPtr(tokens, i++);

                ASSERT(lparen_tok->type == PTOK_L_PAREN);
                ASSERT(ident_tok ->type == PTOK_IDENT  );
                ASSERT(rparen_tok->type == PTOK_R_PAREN);

                AST_Node pred_succ_node;

                if (int_pred_succ_tok->type == PTOK_PRED)
                    pred_succ_node = AST_CreateNode(AST_PRED);
                else
                    pred_succ_node = AST_CreateNode(AST_SUCC);
                
                AST_Node* pred_succ_node_ptr =
                    AST_AddChild(node, &pred_succ_node);
                
                // Vi lägger in variabelindexen för de två variablerna i
                // operationen.
                AST_AddValue(pred_succ_node_ptr, atoi(tok->value+1));
                AST_AddValue(pred_succ_node_ptr, atoi(ident_tok->value+1));
            }
            else {
                // Detta ska aldrig kunna ske efter verifierad syntax.
                FAIL();
            }

            break;
        }

        /*----------------------------------------------------
         * WHILE <variabel> != 0 DO ... END
         *--------------------------------------------------*/
        case PTOK_WHILE: {
            P_Token* ident_tok   = Array_GetElemPtr(tokens, i++);
            P_Token* eq_test_tok = Array_GetElemPtr(tokens, i++);
            P_Token* int_tok     = Array_GetElemPtr(tokens, i++);
            P_Token* do_tok      = Array_GetElemPtr(tokens, i++);
            P_Token* end_tok     = Array_GetElemPtr(tokens, i++);

            ASSERT(ident_tok  ->type == PTOK_IDENT  );
            ASSERT(eq_test_tok->type == PTOK_EQ_TEST);
            ASSERT(int_tok    ->type == PTOK_INT    );
            ASSERT(do_tok     ->type == PTOK_DO     );

            AST_Node  while_node     = AST_CreateNode(AST_WHILE);
            AST_Node* while_node_ptr = AST_AddChild(node, &while_node);

            AST_AddValue(while_node_ptr, atoi(ident_tok->value+1));

            // Om det inte är en tom loop så går vi in i loopen och hanterar
            // alla tokens där.
            if (end_tok->type != PTOK_END)
                i = ParseTokens(while_node_ptr, tokens, i-1);
            
            break;
        }

        /*----------------------------------------------------
         * END
         *--------------------------------------------------*/
        case PTOK_END: {
            // Vi har nått det syntaktiska slutet av en while-loop.
            return i;
        }

        /*----------------------------------------------------
         * RESULT (<variabel>)
         *--------------------------------------------------*/
        case PTOK_RESULT: {
            P_Token* lparen_tok = Array_GetElemPtr(tokens, i++);
            P_Token* ident_tok  = Array_GetElemPtr(tokens, i++);
            P_Token* rparen_tok = Array_GetElemPtr(tokens, i++);
            P_Token* eof_tok    = Array_GetElemPtr(tokens, i);

            ASSERT(lparen_tok->type == PTOK_L_PAREN);
            ASSERT(ident_tok ->type == PTOK_IDENT  );
            ASSERT(rparen_tok->type == PTOK_R_PAREN);
            ASSERT(eof_tok   ->type == PTOK_EOF    );

            AST_Node  result_node     = AST_CreateNode(AST_RESULT);
            AST_Node* result_node_ptr = AST_AddChild(node, &result_node);

            // Vi lägger in index på den variabel som ska vara output.
            AST_AddValue(result_node_ptr, atoi(ident_tok->value+1));
            return i;
        }

        default:
            // Detta ska aldrig kunna ske efter verifierad syntax.
            FAIL();
        }
    }

    // Hit kommer vi aldrig.
    FAIL();
}

/*--------------------------------------
 * Function: AST_CreateNode()
 * Parameters:
 *   type  Typen av noden som ska skapas.
 *
 * Description:
 *   Den här funktionen skapar en AST-nod.
 *------------------------------------*/
AST_Node AST_CreateNode(AST_Node_Type type) {
    AST_Node node;
    
    node.type   = type;
    node.parent = NULL;

    Array_Init(&node.children, sizeof(AST_Node));
    Array_Init(&node.values  , sizeof(int));

    return node;
}

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
AST_Node AST_GenerateTree(const Array* tokens) {
    int i = 0;

    P_Token* program_tok = Array_GetElemPtr(tokens, i++);
    P_Token* lparen_tok  = Array_GetElemPtr(tokens, i++);

    ASSERT(program_tok->type == PTOK_PROGRAM);
    ASSERT(lparen_tok ->type == PTOK_L_PAREN);

    AST_Node program_node = AST_CreateNode(AST_PROGRAM);

    while (TRUE) {
        P_Token* ident_tok = Array_GetElemPtr(tokens, i++);
        P_Token* comma_tok = Array_GetElemPtr(tokens, i++);

        ASSERT(ident_tok->type == PTOK_IDENT);

        // Vi lägger till variabelindexet som input-värde.
        AST_AddValue(&program_node, atoi(ident_tok->value+1));

        // Om det inte är ett komma så finns inga fler variabler i input-listan.
        if (comma_tok->type != PTOK_COMMA) {
            i--;
            break;
        }
    }

    P_Token* rparen_tok = Array_GetElemPtr(tokens, i++);

    ASSERT(rparen_tok->type == PTOK_R_PAREN);

    ParseTokens(&program_node, tokens, i);

    return program_node;
}

/*--------------------------------------
 * Function: AST_PrintNode()
 * Parameters:
 *   node    Noden varifrån trädet ska skrivas ut.
 *   indent  Indentering i antal mellanslag.
 *
 * Description:
 *   Skriver ut det abstrakta syntax-trädet.
 *------------------------------------*/
void AST_PrintNode(const AST_Node* node, int indent) {
    // Den här funktionen är inte så vettig egentligen, men den är ok för att
    // visualisera syntax-träd.

    switch (node->type) {
    case AST_ASSIGN: {
        int var = *(int*)Array_GetElemPtr(&node->values, 0);
        int val = *(int*)Array_GetElemPtr(&node->values, 1);
        printf("assign x%d %d\n", var, val);
        break;
    }

    case AST_PRED: {
        int var0 = *(int*)Array_GetElemPtr(&node->values, 0);
        int var1 = *(int*)Array_GetElemPtr(&node->values, 1);
        printf("pred x%d x%d\n", var0, var1);
        break;
    }

    case AST_PROGRAM: {
        printf("program ");

        int num_inputs = Array_Length(&node->values);
        for (int i = 0; i < num_inputs; i++) {
            int var = *(int*)Array_GetElemPtr(&node->values, i);
            if (i > 0)
                printf(" ");
            printf("x%d", var);
        }

        printf("\n");

        int num_children = Array_Length(&node->children);
        for (int i = 0; i < num_children; i++) {
            AST_Node* child = Array_GetElemPtr(&node->children, i);

            for (int i = 0; i < indent; i++)
                printf(" \xb3  ");

            if (Array_Length(&child->children)==0 && i==(num_children-1))
                printf(" \xc0\xc4\xc4");
            else
                printf(" \xc3\xc4\xc4");

            AST_PrintNode(child, indent+1);
        }

        break;
    }

    case AST_RESULT: {
        int var = *(int*)Array_GetElemPtr(&node->values, 0);
        printf("result x%d\n", var);
        break;
    }

    case AST_SUCC: {
        int var0 = *(int*)Array_GetElemPtr(&node->values, 0);
        int var1 = *(int*)Array_GetElemPtr(&node->values, 1);
        printf("succ x%d x%d\n", var0, var1);
        break;
    }

    case AST_WHILE: {
        int var = *(int*)Array_GetElemPtr(&node->values, 0);

        printf("while x%d\n", var);

        int num_children = Array_Length(&node->children);
        for (int i = 0; i < num_children; i++) {
            AST_Node* child = Array_GetElemPtr(&node->children, i);

            for (int i = 0; i < indent; i++)
                printf(" \xb3  ");

            if (Array_Length(&child->children)==0 && i==(num_children-1))
                printf(" \xc0\xc4\xc4");
            else
                printf(" \xc3\xc4\xc4");

            AST_PrintNode(child, indent + 1);
        }

        break;
    }

    default:
        FAIL();
    }
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
void AST_Repair(AST_Node* node) {
    int num_children = Array_Length(&node->children);
    for (int i = 0; i < num_children; i++) {
        AST_Node* child = Array_GetElemPtr(&node->children, i);
        child->parent = node;
        AST_Repair(child);
    }
}

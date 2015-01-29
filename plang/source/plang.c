/*------------------------------------------------------------------------------
 * File: plang.c
 * Created: January 2, 2015
 * Last changed: January 12, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Detta är huvudfilen för plang, som knyter samman alla andra moduler.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "array.h"
#include "asm.h"
#include "ast.h"
#include "debug.h"
#include "io.h"
#include "tokenizer.h"
#include "string.h"
#include "syntax.h"
#include "vm.h"

#include <time.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: CMD_ASM
 *
 * Description:
 *   Det här kommandot innebär att vi kompilerar P-programmet till assembly-x86-
 *   kod.
 *------------------------------------*/
#define CMD_ASM 1

/*--------------------------------------
 * Constant: CMD_COMPILE
 *
 * Description:
 *   Det här kommandot innebär att vi kompilerar P-programmet till assembly-x86-
 *   kod, för att sedan generera en exe-fil.
 *------------------------------------*/
#define CMD_COMPILE 2

/*--------------------------------------
 * Constant: CMD_PRINT_AST
 *
 * Description:
 *   Det här kommandot innebär att vi skriver ut syntax-trädet utan att göra
 *   något annat utöver det.
 *------------------------------------*/
#define CMD_PRINT_AST 3

/*--------------------------------------
 * Constant: CMD_RUN_VM
 *
 * Description:
 *   Det här kommandot innebär att vi laddar in källkoden, genererar ett AST och
 *   därefter kör programmet i en virtuell maskin.
 *------------------------------------*/
#define CMD_RUN_VM 4

/*--------------------------------------
 * Constant: CMD_SYN_CHECK
 *
 * Description:
 *   Det här kommandot innebär att vi laddar in källkoden och kontrollerar
 *   syntax.
 *------------------------------------*/
#define CMD_SYN_CHECK 5

/*--------------------------------------
 * Constant: ERR_IO_ERROR
 *
 * Description:
 *   Exit-värde som indikerar I/O-fel.
 *------------------------------------*/
#define ERR_IO_ERROR 1

/*--------------------------------------
 * Constant: ERR_SYNTAX_ERROR
 *
 * Description:
 *   Exit-värde som indikerar syntaxfel.
 *------------------------------------*/
#define ERR_SYNTAX_ERROR 2

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: ChangeFileExt()
 * Parameters:
 *   file_name Filnamnet vars filändelse ska ändras.
 *   file_ext  Den nya filändelsen.
 *
 *
 * Description:
 *   Byter filändelse på en sträng och returnerar det som en ny sträng. Glöm
 *   inte att anropa free()!
 *------------------------------------*/
static char* ChangeFileExt(const char* file_name, const char* file_ext) {
    int fn_len  = Str_Length(file_name);
    int ext_len = Str_Length(file_ext);

    char* s;

    for (int i = fn_len-1; i >= 0; i--) {
        if (file_name[i] != '.')
            continue;

        // Vi behöver allokera upp till i, plus en punkt, plus längden på
        // filtillägget, plus null-char.
        int len = i + 1 + ext_len + 1;
        s = malloc(len);

        char* s2 = s;
        for (int j = 0; j < i; j++)
            *(s2++) = file_name[j];
        *(s2++) = '.';
        for (int j = 0; j < ext_len; j++)
            *(s2++) = file_ext[j];
        *(s2++) = '\0';

        return s;
    }

    // Hit kommer vi aldrig.
    FAIL(); return NULL;
}

/*--------------------------------------
 * Function: PrintLogo()
 * Parameters:
 *
 * Description:
 *   Skriver ut programmets namn m.m.
 *------------------------------------*/
static void PrintLogo() {
    printf("plang  version %s - The P Language Compiler\n  by %s\n\n",
           PLANG_PROGRAM_VERSION, PLANG_PROGRAM_AUTHORS);
}

/*--------------------------------------
 * Function: PrintUsage()
 * Parameters:
 *
 * Description:
 *   Skriver ut information om hur programmet används.
 *------------------------------------*/
static void PrintUsage() {
    printf(
        "Usage: plang [command|filename] [filename] [-debug|-no-opt]"       "\n"
        ""                                                                  "\n"
        "Commands:"                                                         "\n"
        ""                                                                  "\n"
        "  -asm       Generates assembly code for the specified input"      "\n"
        "             source file."                                         "\n"
        ""                                                                  "\n"
        "  -printast  Displays the abstract syntax tree generated by"       "\n"
        "             the source code in the specificed input file."        "\n"
        ""                                                                  "\n"
        "  -compile   Compiles the specified input source file into a"      "\n"
        "             runnable executable file. Specify -no-opt after the"  "\n"
        "             filename to disable code optimizations."              "\n"
        ""                                                                  "\n"
        "  -runvm     Runs the specified input source file in a virtual."   "\n"
        "             machine. Specify -debug to step through the program"  "\n"
        "             and print out the variable values as they change."    "\n"
        ""                                                                  "\n"
        "  -syncheck  Loads the source code from the specified input file"  "\n"
        "             and performs a syntax check."                         "\n"
        ""                                                                  "\n"
    );
}

/*--------------------------------------
 * Function: main()
 * Parameters:
 *   argc  Antal argument i kommandoraden.
 *   argv  Vektor innehållande argumenten i kommandoraden.
 *
 * Description:
 *   Programmets huvudfunktion.
 *------------------------------------*/
int main(int argc, char* argv[]) {
    PrintLogo();

    int   command;
    char* file_name;
    Bool  pause_on_exit = FALSE;

    if (argc == 1) {
        // Ingen kommandorad specificerad, så vi skriver ut information om hur
        // man använder plang och låter användaren specificera en fil att köra i
        // en virtuell maskin.
        PrintUsage();

        printf("Enter name of P source file to run: ");

        command       = CMD_RUN_VM;
        file_name     = IO_GetStrFromUser();
        pause_on_exit = TRUE;
    }
    else if (argc == 2) {
        // Endast ett argument på kommandoraden, förmodligen har användaren
        // dragit en fil med muspekaren till plang, så vi utgår ifrån att
        // argumentet är ett filnamn och väljer att köra den angivna filen i en
        // virtuell maskin.
        PrintUsage();

        command       = CMD_RUN_VM;
        file_name     = Str_Duplicate(argv[1]);
        pause_on_exit = TRUE;
    }
    else if (argc >= 3) {
             if (Str_Compare(argv[1], "-asm"     )==0) command = CMD_ASM;
        else if (Str_Compare(argv[1], "-compile" )==0) command = CMD_COMPILE;
        else if (Str_Compare(argv[1], "-printast")==0) command = CMD_PRINT_AST;
        else if (Str_Compare(argv[1], "-runvm"   )==0) command = CMD_RUN_VM;
        else if (Str_Compare(argv[1], "-syncheck")==0) command = CMD_SYN_CHECK;

        file_name = Str_Duplicate(argv[2]);
    }

    printf("Source file: %s\n", file_name);

    char* source_code = IO_ReadFile(file_name);

    if (!source_code) {
        free(file_name);
        printf("ERROR: Could not load source file.\n");
        if (pause_on_exit)
            IO_Pause();
        return ERR_IO_ERROR;
    }

    Array errors; Array_Init(&errors, sizeof(Syntax_Error));
    Array tokens; Array_Init(&tokens, sizeof(P_Token));

    /*----------------------------------------------------
     * 1. Dela upp källkoden i s.k. tokens.
     *--------------------------------------------------*/
    Tok_Tokenize(source_code, &tokens);

    /*----------------------------------------------------
     * 2. Kontrollera att syntaxen är korrekt.
     *--------------------------------------------------*/
    Syn_CheckSyntax(&tokens, &errors, source_code);

    int num_errors = Array_Length(&errors);
    if (num_errors > 0) {
        printf("\n");

        int num_actual_errors = 0;
        int num_warnings      = 0;
        for (int i = 0; i < num_errors; i++) {
            Syntax_Error* err = Array_GetElemPtr(&errors, i);

            Syn_PrintError(err);
            printf("------------------------\n");

            if (!err->is_warning)
                num_actual_errors++;
            else
                num_warnings++;
        }

        // Släpp felmeddelandena ur minnet.
        for (int i = 0; i < num_errors; i++) {
            Syntax_Error* err = Array_GetElemPtr(&errors, i);
            free(err->text);
        }

        Array_Free(&errors);

        printf("\n%d errors, %d warnings\n\n", num_actual_errors, num_warnings);

        if (num_actual_errors > 0) {
            Array_Free(&tokens);
            free(source_code);
            free(file_name);
            if (pause_on_exit)
                IO_Pause();
            return ERR_SYNTAX_ERROR;
        }
    }

    if (command == CMD_SYN_CHECK) {
        // Kommandot innebär att vi bara ska kontrollera syntaxen, så vi är
        // klara här.
        Array_Free(&tokens);
        free(source_code);
        free(file_name);
        if (pause_on_exit)
            IO_Pause();
        return 0;
    }

    /*----------------------------------------------------
     * 3. Generera syntax-trädet.
     *--------------------------------------------------*/
    AST_Node syntax_tree = AST_GenerateTree(&tokens);

    // Vi måste anropa AST_Repair() här pga att trädet kopierats från funktionen
    // AST_GenerateTree() till nuvarande stack.
    AST_Repair(&syntax_tree);

    switch (command) {
    /*----------------------------------------------------
     * 4a. Kompilera syntax-trädet till assembly-kod och
     *     generera eventuellt en exe-fil.
     *--------------------------------------------------*/
    case CMD_ASM:
    case CMD_COMPILE: {
        Bool optimize = !(argc>3 && Str_Compare(argv[3], "-no-opt")==0);
        if (!optimize)
            printf("Code optimizations disabled.\n");

        char* asm_file = ChangeFileExt(file_name, "asm");
        
        // Först genererar vi assembly-koden...
        Asm_GenerateCode(&syntax_tree, asm_file, optimize);

        if (command == CMD_COMPILE) {
            printf("\n");

            char cmd_str[1024];
            sprintf(cmd_str, "fasm\\fasm \"%s\"", asm_file);

            // ...sedan anropar vi fasm för att assemblera koden till en exe.
            system(cmd_str);
        }

        free(asm_file);
        break;
    }

    /*----------------------------------------------------
     * 4b. Skriv ut det abstrakta syntax-trädet.
     *--------------------------------------------------*/
    case CMD_PRINT_AST:
        printf("\n");
        AST_PrintNode(&syntax_tree, 0);
        break;

    /*----------------------------------------------------
     * 4c. Kör syntax-trädet i en virtuell maskin.
     *--------------------------------------------------*/
    case CMD_RUN_VM: {
        Bool debug = (argc>3 && Str_Compare(argv[3], "-debug")==0);
#   ifdef DEBUG
        debug = TRUE;
#   endif
        if (debug)
            printf("Debug mode enabled.\n");

        VM_Config vm_conf;

        // Nolla alla variabler.
        for (int i = 0; i < PLANG_NUM_VARS; i++)
            vm_conf.vars[i] = 0;

        // Låt användaren skriva in input-värdena.
        int num_inputs = Array_Length(&syntax_tree.values);
        for (int i = 0; i < num_inputs; i++) {
            int var = *(int*)Array_GetElemPtr(&syntax_tree.values, i);

            printf("X%d = ", var);
            vm_conf.vars[var] = IO_GetIntFromUser();
        }

        printf("\nRunning program, please wait...\n");

        vm_conf.enable_debug = debug;

        clock_t start   = clock();
        int     result  = VM_ExecAST(&syntax_tree, &vm_conf);
        clock_t finish  = clock();
        int     time_ms = (1000 * (finish - start)) / CLOCKS_PER_SEC;

        if (result == VM_ERR_INF_LOOP) {
            printf("\nERROR: Program got stuck in an infinite loop.\n");
            VM_StateDump(&syntax_tree, 0, &vm_conf);
            printf("\nVM state dump!\n");
        }
        else if (result == VM_ERR_INVALID_VAR) {
            printf("\nERROR: Attempted to use an invalid variable.\n");
            VM_StateDump(&syntax_tree, 0, &vm_conf);
            printf("\nVM state dump!\n");
        }
        else if (result == VM_ERR_OVERFLOW) {
            printf("\nERROR: A variable overflowed.\n\n");
            VM_StateDump(&syntax_tree, 0, &vm_conf);
            printf("\nVM state dump!\n");
        }
        else if (result == VM_ERR_PREMATURE_RESULT) {
            printf("\nERROR: Premature RESULT node encountered.\n");
            VM_StateDump(&syntax_tree, 0, &vm_conf);
            printf("\nVM state dump!\n");
        }
        else {
            if (!debug)
                printf("Done! Execution time: %d ms\n", time_ms);
            else
                printf("Done!\n");
            printf("\nResult: %d\n\n", result);
        }

        break;
    }

    default:
        printf("Unknown command: %s\n", argv[1]);
        break;
    }

    // TODO: Rensa upp allt minne här.

    Array_Free(&tokens);
    free(source_code);
    free(file_name);

    if (pause_on_exit)
        IO_Pause();
    return 0;
}

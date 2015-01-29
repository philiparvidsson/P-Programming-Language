/*------------------------------------------------------------------------------
 * File: asm.c
 * Created: January 5, 2015
 * Last changed: January 12, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Funktioner för att generera assembly-x86-kod av ett abstrakt syntax-träd
 *   och generera en exe-fil av det. Koden som genereras är något specifik för
 *   flat assembler (fasm).
 *
 * Changes:
 *   * Lade in stöd för optimeringar. Numer skrivs bara mov ebx, _Vars+offs ut
 *     som kod om EBX-registret inte redan pekar mot samma adress.
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "asm.h"
#include "ast.h"
#include "common.h"
#include "debug.h"
#include "vm.h"

#include <stdio.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: Code_Info
 *
 * Description:
 *   Datastruktur som hjälper till att hålla reda på olika variabler och värden
 *   vid kodgenerering.
 *------------------------------------*/
typedef struct {
    Bool enable_optimizations;
    Bool enable_source_comments;
    int  label_counter;
} Code_Info;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: GenerateCode()
 * Parameters:
 *   node  Noden som vi ska generera assembly-x86-kod för.
 *   ci    Hjälpobjekt för kodgenerering.
 *   fp    Pekare till filen som koden ska skrivas ut till.
 *
 * Description:
 *   Genererar assembly-x86-kod för den specificerade noden.
 *------------------------------------*/
static void GenerateCode(const AST_Node* node, Code_Info* ci, FILE* fp) {
    switch (node->type) {
    /*----------------------------------------------------
     * PROGRAM (<variabel>[, <variabel>])
     *--------------------------------------------------*/
    case AST_PROGRAM: {
        fprintf(fp, "  call InitInputBox"    "\n");

        int num_inputs = Array_Length(&node->values);
        for (int i = 0; i < num_inputs; i++) {
            int var = *(int*)Array_GetElemPtr(&node->values, i);

            fprintf(fp, "  push dword %d"        "\n"
                        "  call InputBox"        "\n"
                        "  mov ebx, _Vars+%d"    "\n"
                        "  mov [ebx], eax"       "\n",
                        var, var*sizeof(int));
        }

        // När vi optimerar koden förlitar vi oss på att EDX-registret är noll.
        if (ci->enable_optimizations)
            fprintf(fp, "  xor edx, edx"    "\n");

        int num_children = Array_Length(&node->children);
        for (int i = 0; i < num_children; i++) {
            AST_Node* child = Array_GetElemPtr(&node->children, i);
            GenerateCode(child, ci, fp);
        }

        break;
    }

    /*----------------------------------------------------
     * <variabel> := <naturligt-tal>
     *--------------------------------------------------*/
    case AST_ASSIGN: {
        int var = *(int*)Array_GetElemPtr(&node->values, 0);
        int val = *(int*)Array_GetElemPtr(&node->values, 1);

        if (ci->enable_source_comments)
            fprintf(fp, "; X%d := %d\n", var, val);

        fprintf(fp, "  mov ebx, _Vars+%d"      "\n"
                    "  mov [ebx], dword %d"    "\n",
                    var*sizeof(int), val);

        break;
    }

    /*----------------------------------------------------
     * <variabel> := PRED(<variabel>)
     *--------------------------------------------------*/
    case AST_PRED: {
        int var0 = *(int*)Array_GetElemPtr(&node->values, 0);
        int var1 = *(int*)Array_GetElemPtr(&node->values, 1);

        if (ci->enable_source_comments)
            fprintf(fp, "; X%d := PRED(X%d)\n", var0, var1);

        int label_num = ci->label_counter++;
        if (var0 == var1 && ci->enable_optimizations) {
            // Om vi använder samma variabel två gånger i operationen (ex.
            // X1 := PRED(X1)) kan vi förenkla assembly-koden något.

            fprintf(fp, "  mov ebx, _Vars+%d"               "\n"
                        "  dec dword [ebx]"                 "\n"
                        "  jns .__Var_Not_Negative_%d__"    "\n"
                        "  mov [ebx], edx"                  "\n" // OBS! EDX = 0
                        ".__Var_Not_Negative_%d__:"         "\n",
                        var0*sizeof(int), label_num, label_num);
        }
        else {
            fprintf(fp, "  mov ebx, _Vars+%d"               "\n"
                        "  mov eax, [ebx]"                  "\n"
                        "  dec eax"                         "\n"
                        "  jns .__Var_Not_Negative_%d__"    "\n"
                        "  xor eax, eax"                    "\n"
                        ".__Var_Not_Negative_%d__:"         "\n"
                        "  mov ebx, _Vars+%d"               "\n"
                        "  mov [ebx], eax"                  "\n",
                        var1*sizeof(int), label_num, label_num,
                        var0*sizeof(int));

        }

        break;
    }

    /*----------------------------------------------------
     * <variabel> := SUCC(<variabel>)
     *--------------------------------------------------*/
    case AST_SUCC: {
        int var0 = *(int*)Array_GetElemPtr(&node->values, 0);
        int var1 = *(int*)Array_GetElemPtr(&node->values, 1);

        if (ci->enable_source_comments)
            fprintf(fp, "; X%d := SUCC(X%d)\n", var0, var1);

        if (var0 == var1 && ci->enable_optimizations) {
            fprintf(fp, "  mov ebx, _Vars+%d"    "\n"
                        "  inc dword [ebx]"      "\n",
                        var0*sizeof(int));
        }
        else {
            fprintf(fp, "  mov ebx, _Vars+%d"               "\n"
                        "  mov eax, [ebx]"                  "\n"
                        "  inc eax"                         "\n"
                        "  mov ebx, _Vars+%d"               "\n"
                        "  mov [ebx], eax"                  "\n",
                        var1*sizeof(int), var0*sizeof(int));
        }

        break;
    }

    /*----------------------------------------------------
     * WHILE <variabel> != 0 DO ... END
     *--------------------------------------------------*/
    case AST_WHILE: {
        int var = *(int*)Array_GetElemPtr(&node->values, 0);

        if (ci->enable_source_comments)
            fprintf(fp, "; WHILE X%d != 0 DO\n", var);

        int label_num = ci->label_counter++;
        fprintf(fp, "__While__%d_%d_Do:"         "\n"
                    "  mov ebx, _Vars+%d"        "\n"
                    "  mov eax, [ebx]"           "\n"
                    "  test eax, eax"            "\n"
                    "  jz __While__%d_%d_End"    "\n",
                    var, label_num, var*sizeof(int), var, label_num);

        int num_children = Array_Length(&node->children);
        for (int i = 0; i < num_children; i++) {
            AST_Node* child = Array_GetElemPtr(&node->children, i);
            GenerateCode(child, ci, fp);
        }

        fprintf(fp, "  jmp __While__%d_%d_Do"    "\n"
                    "__While__%d_%d_End:"        "\n",
                    var, label_num, var, label_num);

        if (ci->enable_source_comments)
            fprintf(fp, "; END\n");

        break;
    }

    /*----------------------------------------------------
     * RESULT (<variabel>)
     *--------------------------------------------------*/
    case AST_RESULT: {
        int var = *(int*)Array_GetElemPtr(&node->values, 0);
        if (ci->enable_source_comments)
            fprintf(fp, "; RESULT (X%d)\n", var);

        fprintf(fp, "  mov ebx, _Vars+%d"         "\n"
                    "  mov eax, [ebx]"            "\n"
                    "  push eax"                  "\n"
                    "  push _szStrResultValue"    "\n"
                    "  call itoa"                 "\n"
                    "  push 0"                    "\n"
                    "  push _szStrResult"         "\n"
                    "  push _szStrResult"         "\n"
                    "  push 0"                    "\n"
                    "  call[MessageBoxA]"         "\n"
                    "exit:"                       "\n"
                    "  push 0"                    "\n"
                    "  call[ExitProcess]"         "\n",
                    var*sizeof(int));

        break;
    }

    default:
        FAIL();
    }
}

/*--------------------------------------
 * Function: WriteHeader()
 * Parameters:
 *   fp  Filen som koden ska skrivas till.
 *
 * Description:
 *   Skriver några inledande kommandon till den specificerade filen.
 *------------------------------------*/
static void WriteHeader(FILE* fp) {
    fprintf(fp, "format PE GUI"    "\n"
                "entry start"      "\n"
                ""                 "\n");
}

/*--------------------------------------
 * Function: WriteProcs()
 * Parameters:
 *   fp  Filen som koden ska skrivas till.
 *
 * Description:
 *   Skriver procedurer som behövs till den specificerade filen.
 *------------------------------------*/
static void WriteProcs(FILE* fp) {
    //--------------------------------------
    // Procedure: atoi
    // Parameters:
    //   1: Adressen till strängen som ska göras om till heltal.
    //
    // Description:
    //   Konverterar strängrepresentationen av ett heltal (i bas 10) till ett
    //   heltal och returnerar värdet i EAX-registret.
    //
    //   Anropa på följande vis:
    //
    //   push str
    //   call atoi
    //--------------------------------------
    fprintf(fp, "atoi:"                  "\n"
                "  pop eax"              "\n"
                "  pop ebx"              "\n"
                "  push eax"             "\n"
                "  mov ecx, ebx"         "\n"
                ".move_next:"            "\n"
                "  cmp byte [ecx], 0"    "\n"
                "  je .done"             "\n"
                "  inc ecx"              "\n"
                "  jmp .move_next"       "\n"
                ".done:"                 "\n"
                "  dec ecx"              "\n"
                "  xor eax, eax"         "\n"
                "  inc eax"              "\n"
                "  xor edi, edi"         "\n"
                ".next_digit:"           "\n"
                "  xor edx, edx"         "\n"
                "  mov dl, [ecx]"        "\n"
                "  sub edx, '0'"         "\n"
                "  imul edx, eax"        "\n"
                "  add edi, edx"         "\n"
                "  imul eax, 10"         "\n"
                "  dec ecx"              "\n"
                "  cmp ecx, ebx"         "\n"
                "  jnb .next_digit"      "\n"
                "  mov eax, edi"         "\n"
                "  ret"                  "\n");

    //--------------------------------------
    // Procedure: InitInputBox
    // Parameters:
    //
    // Description:
    //   Initierar fönster-klassen för InputBox-funktionen.
    //
    //   Anropa på följande vis:
    //
    //   call InitInputBox
    //--------------------------------------
    fprintf(fp, "InitInputBox:"                                         "\n"
                "  push ebp"                                            "\n"
                "  mov ebp, esp"                                        "\n"
                "  sub esp, 48 ; wcx"                                   "\n"
                "  mov [ebp-48], dword 48           ; cbSize"           "\n"
                "  mov [ebp-44], dword 2+1          ; style"            "\n"
                "  mov [ebp-40], dword WindowProc   ; lpfnWndProc"      "\n"
                "  mov [ebp-36], dword 0            ; cbClsExtra"       "\n"
                "  mov [ebp-32], dword 0            ; cbWndExtra"       "\n"
                "  mov [ebp-28], dword 0            ; hInstance"        "\n"
                "  mov [ebp-24], dword 0            ; hIcon"            "\n"
                "  mov [ebp-20], dword 0            ; hCursor"          "\n"
                "  mov [ebp-16], dword 5+1          ; hBrush"           "\n"
                "  mov [ebp-12], dword 0            ; lpszMenuName"     "\n"
                "  mov [ebp-8 ], dword _szClassName ; lpszClassName"    "\n"
                "  mov [ebp-4 ], dword 0            ; hIconSm"          "\n"
                "  push esp ; &wcx"                                     "\n"
                "  call [RegisterClassExA]"                             "\n"
                "  mov esp, ebp"                                        "\n"
                "  pop ebp"                                             "\n"
                "  ret"                                                 "\n");

    //--------------------------------------
    // Procedure: InputBox
    // Parameters:
    //   1: Index på inputvariabeln.
    //
    // Description:
    //   Visar en input-ruta så att användaren kan skriva in ett helta. Retur-
    //   värdet lagras i EAX-registret.
    //
    //   Anropa på följande vis:
    //
    //   push 1
    //   call Inputbox
    //--------------------------------------
    fprintf(fp, "InputBox:"                                               "\n"
                "  push ebp"                                              "\n"
                "  mov ebp, esp"                                          "\n"
                "  mov eax, [ebp+8]"                                      "\n"
                "  push eax"                                              "\n"
                "  push _szStaticTextInt"                                 "\n"
                "  call itoa"                                             "\n"
                "  sub esp, 4+28        ; hWnd, msg"                      "\n"
                "  push dword 0         ; lpParam"                        "\n"
                "  push dword 0         ; hInstance"                      "\n"
                "  push dword 0         ; hMenu"                          "\n"
                "  push dword 0         ; hWndParent"                     "\n"
                "  push dword 70        ; nHeight"                        "\n"
                "  push dword 160       ; nWidth"                         "\n"
                "  push dword 80000000h ; y (CW_USEDEFAULT)"              "\n"
                "  push dword 80000000h ; x (CW_USEDEFAULT)"              "\n"
                "  push dword 80000h    ; dwStyle (WS_SYSMENU)"           "\n"
                "  push dword _szWindowName"                              "\n"
                "  push dword _szClassName"                               "\n"
                "  push dword 8h        ; dwExStyle (WS_EX_TOPMOST)"      "\n"
                "  call [CreateWindowExA]"                                "\n"
                "  mov [ebp-4], eax     ; hWnd"                           "\n"
                "  push dword 0         ; lpParam"                        "\n"
                "  push dword 0         ; hInstance"                      "\n"
                "  push dword 101       ; hMenu"                          "\n"
                "  push dword [ebp-4]   ; hWndParent"                     "\n"
                "  push dword 24        ; nHeight"                        "\n"
                "  push dword 28        ; nWidth"                         "\n"
                "  push dword 9         ; y"                              "\n"
                "  push dword 6         ; x"                              "\n"
                "  push dword 50000000h ; dwStyle"                        "\n"
                "  push dword _szStaticText ; lpWindowName"               "\n"
                "  push dword _szStaticClass"                             "\n"
                "  push dword 0h        ; dwExStyle"                      "\n"
                "  call [CreateWindowExA]"                                "\n"
                "  push dword 0         ; lpParam"                        "\n"
                "  push dword 0         ; hInstance"                      "\n"
                "  push dword 101       ; hMenu"                          "\n"
                "  push dword [ebp-4]   ; hWndParent"                     "\n"
                "  push dword 24        ; nHeight"                        "\n"
                "  push dword 84        ; nWidth"                         "\n"
                "  push dword 6         ; y"                              "\n"
                "  push dword 34        ; x"                              "\n"
                "  push dword 50000000h ; dwStyle"                        "\n"
                "  push dword _szZero   ; lpWindowName"                   "\n"
                "  push dword _szEditClass"                               "\n"
                "  push dword 200h      ; dwExStyle (WS_EX_CLIENTEDGE)"   "\n"
                "  call [CreateWindowExA]"                                "\n"
                "  mov [_hWndEdit], eax"                                  "\n"
                "  push 5 ; SW_SHOW"                                      "\n"
                "  push dword [ebp-4]"                                    "\n"
                "  call [ShowWindow]"                                     "\n"
                "  push dword [ebp-4]"                                    "\n"
                "  call [UpdateWindow]"                                   "\n"
                ".msg_loop:"                                              "\n"
                "  push 0 ; wMsgFilterMax"                                "\n"
                "  push 0 ; wMsgFilterMin"                                "\n"
                "  push 0 ; hWnd (NULL)"                                  "\n"
                "  mov eax, ebp"                                          "\n"
                "  sub eax, 32"                                           "\n"
                "  push eax  ; lpMsg"                                     "\n"
                "  call [GetMessageA]"                                    "\n"
                "  test eax, eax"                                         "\n"
                "  jle .done"                                             "\n"
                "  mov eax, ebp"                                          "\n"
                "  sub eax, 32"                                           "\n"
                "  push eax  ; lpMsg"                                     "\n"
                "  push eax  ; lpMsg"                                     "\n"
                "  call [TranslateMessage]"                               "\n"
                "  call [DispatchMessageA]"                               "\n"
                "  jmp .msg_loop"                                         "\n"
                ".done:"                                                  "\n"
                "  push _szEditText"                                      "\n"
                "  call atoi"                                             "\n"
                "  mov esp, ebp"                                          "\n"
                "  pop ebp"                                               "\n"
                "  ret"                                                   "\n");

    //--------------------------------------
    // Procedure: itoa
    // Parameters:
    //   1: Heltalet som ska göras om till en strängrepresentation.
    //   2: Adressen till strängen där resultatet ska lagras.
    //
    // Description:
    //   Konverterar ett heltalsvärde till sin strängrepresentation i bas 10.
    //
    //   Anropa på följande vis:
    //
    //   push 1337
    //   push str
    //   call itoa
    //--------------------------------------
    fprintf(fp, "itoa:"                "\n"
                "  pop ebx"            "\n"
                "  pop ecx"            "\n"
                "  pop eax"            "\n"
                "  push ebx"           "\n"
                "  push ecx"           "\n"
                "  mov ebx, 10"        "\n"
                ".next_digit:"         "\n"
                "  xor edx, edx"       "\n"
                "  div ebx"            "\n"
                "  add dl, '0'"        "\n"
                "  mov [ecx], dl"      "\n"
                "  inc ecx"            "\n"
                "  test eax, eax"      "\n"
                "  jnz .next_digit"    "\n"
                "  xor dl, dl"         "\n"
                "  mov [ecx], dl"      "\n"
                "  dec ecx"            "\n"
                "  pop ebx"            "\n"
                ".reverse:"            "\n"
                "  mov al, [ebx]"      "\n"
                "  mov ah, [ecx]"      "\n"
                "  mov [ebx], ah"      "\n"
                "  mov [ecx], al"      "\n"
                "  inc ebx"            "\n"
                "  dec ecx"            "\n"
                "  cmp ebx, ecx"       "\n"
                "  jb .reverse"        "\n"
                "  ret"                "\n");


    //--------------------------------------
    // Procedure: WindowProc
    // Parameters:
    //   1: hwnd
    //   2: msg
    //   3: wParam
    //   4: lParam
    //
    // Description:
    //   Callback-funktion för fönster.
    //--------------------------------------
    fprintf(fp, "WindowProc:"                       "\n"             
                "  push ebp"                        "\n"
                "  mov ebp, esp"                    "\n"
                "  mov eax, [ebp+12] ; msg"         "\n"
                "  cmp eax, 10h ; WM_CLOSE?"        "\n"
                "  jne .not_wm_close"               "\n"
                "  push 8"                          "\n"
                "  push _szEditText"                "\n"
                "  push dword [_hWndEdit]"          "\n"
                "  call [GetWindowTextA]"           "\n"
                "  push dword [ebp+8] ; hwnd"       "\n"
                "  call [DestroyWindow]"            "\n"
                "  xor eax, eax"                    "\n"
                "  jmp .done"                       "\n"
                ".not_wm_close:"                    "\n"
                "  cmp eax, 2h ; WM_DESTROY?"       "\n"
                "  jne .not_wm_destroy"             "\n"
                "  push 0"                          "\n"
                "  call [PostQuitMessage]"          "\n"
                "  xor eax, eax"                    "\n"
                "  jmp .done"                       "\n"
                ".not_wm_destroy:"                  "\n"
                "  push dword [ebp+20] ; lParam"    "\n"
                "  push dword [ebp+16] ; wParam"    "\n"
                "  push dword [ebp+12] ; msg"       "\n"
                "  push dword [ebp+8 ] ; hWNd"      "\n"
                "  call [DefWindowProcA]"           "\n"
                ".done:"                            "\n"
                "  mov esp, ebp"                    "\n"
                "  pop ebp"                         "\n"
                "  ret"                             "\n");
}

/*--------------------------------------
 * Function: WriteSectData()
 * Parameters:
 *   fp  Filen som koden ska skrivas till.
 *
 * Description:
 *   Skriver sektionen '.data' till den specificerade filen.
 *------------------------------------*/
static void WriteSectData(FILE* fp) {
    fprintf(fp, "section '.idata' import data readable writeable"    "\n"
            ""                                                       "\n"
            "  dd 0, 0, 0, RVA kernel_name, RVA kernel_table"        "\n"
            "  dd 0, 0, 0, RVA user_name, RVA user_table"            "\n"
            "  dd 0, 0, 0, 0, 0"                                     "\n"
            ""                                                       "\n"
            "kernel_table:"                                          "\n"
            "  ExitProcess dd RVA _ExitProcess"                      "\n"
            "  GetLastError dd RVA _GetLastError"                    "\n"
            "  dd 0"                                                 "\n"
            ""                                                       "\n"
            "user_table:"                                            "\n"
            "  CreateWindowExA dd RVA _CreateWindowExA"              "\n"
            "  DefWindowProcA dd RVA _DefWindowProcA"                "\n"
            "  DestroyWindow dd RVA _DestroyWindow"                  "\n"
            "  DispatchMessageA dd RVA _DispatchMessageA"            "\n"
            "  GetMessageA dd RVA _GetMessageA"                      "\n"
            "  GetWindowTextA dd RVA _GetWindowTextA"                "\n"
            "  MessageBoxA dd RVA _MessageBoxA"                      "\n"
            "  PostQuitMessage dd RVA _PostQuitMessage"              "\n"
            "  RegisterClassExA dd RVA _RegisterClassExA"            "\n"
            "  ShowWindow dd RVA _ShowWindow"                        "\n"
            "  TranslateMessage dd RVA _TranslateMessage"            "\n"
            "  UpdateWindow dd RVA _UpdateWindow"                    "\n"
            "  dd 0"                                                 "\n"
            ""                                                       "\n"
            "  kernel_name db 'KERNEL32.DLL', 0"                     "\n"
            "  user_name   db 'USER32.DLL', 0"                       "\n"
            ""                                                       "\n"
            "  _ExitProcess dw 0"                                    "\n"
            "  db 'ExitProcess', 0"                                  "\n"
            "  _GetLastError dw 0"                                   "\n"
            "  db 'GetLastError', 0"                                 "\n"
            ""                                                       "\n"
            "  _CreateWindowExA dw 0"                                "\n"
            "  db 'CreateWindowExA', 0"                              "\n"
            "  _DefWindowProcA dw 0"                                 "\n"
            "  db 'DefWindowProcA', 0"                               "\n"
            "  _DestroyWindow dw 0"                                  "\n"
            "  db 'DestroyWindow', 0"                                "\n"
            "  _DispatchMessageA dw 0"                               "\n"
            "  db 'DispatchMessageA', 0"                             "\n"
            "  _GetMessageA dw 0"                                    "\n"
            "  db 'GetMessageA', 0"                                  "\n"
            "  _GetWindowTextA dw 0"                                 "\n"
            "  db 'GetWindowTextA', 0"                               "\n"
            "  _MessageBoxA dw 0"                                    "\n"
            "  db 'MessageBoxA', 0"                                  "\n"
            "  _PostQuitMessage dw 0"                                "\n"
            "  db 'PostQuitMessage', 0"                              "\n"
            "  _RegisterClassExA dw 0"                               "\n"
            "  db 'RegisterClassExA', 0"                             "\n"
            "  _ShowWindow dw 0"                                     "\n"
            "  db 'ShowWindow', 0"                                   "\n"
            "  _TranslateMessage dw 0"                               "\n"
            "  db 'TranslateMessage', 0"                             "\n"
            "  _UpdateWindow dw 0"                                   "\n"
            "  db 'UpdateWindow', 0"                                 "\n"
            ""                                                       "\n"
            ""                                                       "\n");
}

/*--------------------------------------
 * Function: WriteSectIdata()
 * Parameters:
 *   fp  Filen som koden ska skrivas till.
 *
 * Description:
 *   Skriver sektionen '.idata' till den specificerade filen.
 *------------------------------------*/
static void WriteSectIdata(FILE* fp) {
    fprintf(fp, "section '.data' data readable writeable"    "\n"
                ""                                           "\n"
                "  _Vars             rd %d"                  "\n"
                "  _szStrResult      db 'Result: '"          "\n"
                "  _szStrResultValue rb 64"                  "\n"
                "  _hWndEdit dd 0"                           "\n"
                "  _szWindowName db 'Input value', 0"        "\n"
                "  _szClassName db 'InputBoxClass', 0"       "\n"
                "  _szEditClass db 'EDIT', 0"                "\n"
                "  _szStaticClass db 'STATIC', 0"            "\n"
                "  _szZero db '0', 0"                        "\n"
                "  _szEditText rb 8"                         "\n"
                "  _szStaticText db 'X'"                     "\n"
                "  _szStaticTextInt rb 8"                    "\n"
                ""                                           "\n"
                ""                                           "\n",
                PLANG_NUM_VARS);
}

/*--------------------------------------
 * Function: WriteSectReloc()
 * Parameters:
 *   fp  Filen som koden ska skrivas till.
 *
 * Description:
 *   Skriver sektionen '.reloc' till den specificerade filen.
 *------------------------------------*/
static void WriteSectReloc(FILE* fp) {
    // Tydligen behövs den här sektionen även om den är tom..?
    fprintf(fp, "section '.reloc' fixups data readable discardable"    "\n"
                 ""                                                    "\n"
                 ""                                                    "\n");
}

/*--------------------------------------
 * Function: WriteSectText()
 * Parameters:
 *   fp  Filen som koden ska skrivas till.
 *
 * Description:
 *   Skriver sektionen '.text' till den specificerade filen.
 *------------------------------------*/
static void WriteSectText(FILE* fp) {
    fprintf(fp, "section '.text' code readable executable"    "\n"
                ""                                            "\n"
                "start:"                                      "\n");
}

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
                      Bool optimize)
{
    FILE* fp = fopen(file_name, "w");

    if (!fp)
        return FALSE;

    Code_Info ci;
    
    ci.enable_optimizations   = optimize;
    ci.label_counter          = 0;
#ifdef DEBUG
    ci.enable_source_comments = TRUE;
#else
    ci.enable_source_comments = FALSE;
#endif

    WriteHeader   (fp);
    WriteSectText(fp);

    GenerateCode(root, &ci, fp);

    WriteProcs(fp);
    WriteSectData (fp);
    WriteSectIdata(fp);
    WriteSectReloc(fp);

    fclose(fp);

    return TRUE;
}

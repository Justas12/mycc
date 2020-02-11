#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../include/backend/code/instrs.h"
#include "../../../include/backend/vm/vm.h"

void vm_exit(VM* vm, int status) {
    delVM(vm);
    printf("\nVM terminated with status %d (", status);
    status == 1 ? printf("error)\n") : printf("success)\n");
    exit(status);
}

void PUSH(VM* vm, int v) {
    if (vm->sp == STACK_SIZE)
        vm_exit(vm, 1);
    vm->stack[++vm->sp] = v;
}

int POP(VM* vm) {
    return vm->stack[vm->sp--];
}

int NCODE(VM* vm) {
    return vm->code[vm->pc++];
}

void PRINTSTACK(VM* vm, int height) {
    for (int i = height; i >= 0; --i) {
        if (vm->sp == i)
            printf("|%d|<-- SP\n", vm->stack[i]);
        else
            printf("|%d|\n", vm->stack[i]);
    }
}

void print_string_table(VM* vm) {
    for (int i = 0; vm->string_table[i] != NULL; ++i)
        printf("%s\n", vm->string_table[i]);
}

int add_float(VM* vm, Float f) {
    static int index = 0;
    vm->float_table[index] = f;
    index = index + 1;
    return index - 1;
}

int add_string(VM* vm, char* s) {
    static int index = 0;
    for (int i = 0; i < index; ++i) {
        if (strcmp(vm->string_table[i], s) == 0)
            return i;
    }
    vm->string_table[index] = s;
    index = index + 1;
    return index - 1;
}

VM* newVM(int* code,    // pointer to table containing a bytecode to be executed
    int pc,             // address of instruction to be invoked as first one - entrypoint/main func
    int datasize) {      // total locals size required to perform a program operations
    VM* vm = (VM*)malloc(sizeof(VM));
    vm->code = code;
    vm->pc = pc;
    vm->fp = 0;
    vm->sp = -1;
    vm->locals = (int*)malloc(sizeof(int) * datasize);
    vm->stack = (int*)malloc(sizeof(int) * STACK_SIZE);
    return vm;
}

void delVM(VM* vm){
    free(vm->locals);
    free(vm->stack);
    free(vm);
}

void run(VM* vm) {
    int running = 1;
    do{
        int opcode = NCODE(vm);        // fetch
        int v, addr, offset, a, b, argc, rval;
        switch(opcode) {
            case I_INT_ADD:
            {
               b = POP(vm);        // get second value from top of the stack ...
               a = POP(vm);        // ... then get first value from top of the stack ...
               PUSH(vm, a + b);    // ... add those two values and put result on top of the stack
               break;
            }
            case I_INT_SUB:
            {
               b = POP(vm);        // get second value from top of the stack ...
               a = POP(vm);        // ... then get first value from top of the stack ...
               PUSH(vm, a - b);    // ... add those two values and put result on top of the stack
               break;
            }
            case I_INT_MULT:
            {
               b = POP(vm);        // get second value from top of the stack ...
               a = POP(vm);        // ... then get first value from top of the stack ...
               PUSH(vm, a * b);    // ... add those two values and put result on top of the stack
               break;
            }
            case I_INT_DIV:
            {
               b = POP(vm);        // get second value from top of the stack ...
               a = POP(vm);        // ... then get first value from top of the stack ...
               PUSH(vm, a / b);    // ... add those two values and put result on top of the stack
               break;
            }
            case I_INT_MOD:
            {
               b = POP(vm);        // get second value from top of the stack ...
               a = POP(vm);        // ... then get first value from top of the stack ...
               PUSH(vm, a % b);    // ... add those two values and put result on top of the stack
               break;
            }
            case I_FLOAT_ADD:
            {
                int slot1 = POP(vm);
                int slot2 = POP(vm);
                Float float1 = vm->float_table[slot1];
                Float float2 = vm->float_table[slot2];
                Float res;
                res.val = float1.val + float2.val;
                int res_slot = add_float(vm, res);
                PUSH(vm, res_slot);
                break;
            }
            case I_FLOAT_SUB:
            {
                int slot1 = POP(vm);
                int slot2 = POP(vm);
                Float float1 = vm->float_table[slot1];
                Float float2 = vm->float_table[slot2];
                Float res;
                res.val = float2.val - float1.val;
                int res_slot = add_float(vm, res);
                PUSH(vm, res_slot);
                break;
            }
            case I_FLOAT_MULT:
            {
                int slot1 = POP(vm);
                int slot2 = POP(vm);
                Float float1 = vm->float_table[slot1];
                Float float2 = vm->float_table[slot2];
                Float res;
                res.val = float1.val * float2.val;
                int res_slot = add_float(vm, res);
                PUSH(vm, res_slot);
                break;
            }
            case I_FLOAT_DIV:
            {
                int slot1 = POP(vm);
                int slot2 = POP(vm);
                Float float1 = vm->float_table[slot1];
                Float float2 = vm->float_table[slot2];
                Float res;
                res.val = float2.val / float1.val;
                int res_slot = add_float(vm, res);
                PUSH(vm, res_slot);
                break;
            }
            case I_INT_LESSER:
            {
                b = POP(vm);
                a = POP(vm);
                a < b ? PUSH(vm, 1) : PUSH(vm, 0);
                break;
            }
            case I_INT_LESSEREQ:
            {
                b = POP(vm);
                a = POP(vm);
                a <= b ? PUSH(vm, 1) : PUSH(vm, 0);
                break;
            }
            case I_INT_GREATER:
            {
                b = POP(vm);
                a = POP(vm);
                a > b ? PUSH(vm, 1) : PUSH(vm, 0);
                break;
            }
            case I_INT_GRESTEREQ:
            {
                b = POP(vm);
                a = POP(vm);
                a >= b ? PUSH(vm, 1) : PUSH(vm, 0);
                break;
            }
            case I_FLOAT_LESSER:
            {
                int slot1 = POP(vm);
                int slot2 = POP(vm);
                Float float1 = vm->float_table[slot1];
                Float float2 = vm->float_table[slot2];
                float1.val > float2.val ? PUSH(vm, 1) : PUSH(vm, 0);
                break;
            }
            case I_FLOAT_LESSEREQ:
            {
                int slot1 = POP(vm);
                int slot2 = POP(vm);
                Float float1 = vm->float_table[slot1];
                Float float2 = vm->float_table[slot2];
                float1.val >= float2.val ? PUSH(vm, 1) : PUSH(vm, 0);
                break;
            }
            case I_FLOAT_GREATER:
            {
                int slot1 = POP(vm);
                int slot2 = POP(vm);
                Float float1 = vm->float_table[slot1];
                Float float2 = vm->float_table[slot2];
                float1.val < float2.val ? PUSH(vm, 1) : PUSH(vm, 0);
                break;
            }
            case I_FLOAT_GREATEREQ:
            {
                int slot1 = POP(vm);
                int slot2 = POP(vm);
                Float float1 = vm->float_table[slot1];
                Float float2 = vm->float_table[slot2];
                float1.val <= float2.val ? PUSH(vm, 1) : PUSH(vm, 0);
                break;
            }
            case I_POP:
            {
                --vm->sp;
                break;
            }
            case I_INT_PUSH:
            {
                v = NCODE(vm);   // get next value from code ...
                PUSH(vm, v);     // ... and move it on top of the stack
                break;
            }
            case I_FLOAT_PUSH:
            {
                Float f;        // utils.c
                int slot;
                f.bytes[0] = NCODE(vm);   // 1st byte ...
                f.bytes[1] = NCODE(vm);   // 2nd byte ...
                f.bytes[2] = NCODE(vm);   // 3rd byte ...
                f.bytes[3] = NCODE(vm);   // 4th byte ...
                slot =  add_float(vm, f);
                PUSH(vm, slot);
                break;
            }
            case I_STRING_PUSH:
            {
                char* str = malloc(sizeof(char)*256);
                char s[256];        // silly!
                int index = 0;
                int slot;
                while((v = NCODE(vm)) != '\0') {
                    s[index++] = v;
                }
                s[index] = v;
                strcpy(str, s);
                slot = add_string(vm, str);
                PUSH(vm, slot);
                break;
            }
            case I_GET_L:
            {
                offset = NCODE(vm);     // get next value from code to identify local variables offset start on the stack
                if (offset < 0) {   // function args
                    PUSH(vm, vm->stack[vm->fp+offset]); // ... put on the top of the stack variable stored relatively to frame pointer  
                }
                else {      // function locals
                    PUSH(vm, vm->locals[vm->fp+offset]);
                }
                break;
            }
            case I_SET_L:
            {
                offset = NCODE(vm);     // ... get the relative pointer address from code ...
                if (offset < 0) {   // function args
                    vm->stack[vm->fp+offset] = POP(vm); // ... put on the top of the stack variable stored relatively to frame pointer  
                }
                else {      // function locals
                    vm->locals[vm->fp+offset] = POP(vm);
                }
                break;
            }
            case I_BR:
            {
                addr = NCODE(vm);
                vm->pc = addr;
                // printf(" (Jumping to: %d)", addr);
                break;
            }
            case I_BZ:
            {
                addr = NCODE(vm);
                if (POP(vm) == 0) {
                    // printf(" (Jumping to: %d)", addr);
                    vm->pc = addr;
                }
                break;
            }
            case I_RET:
            {
                rval = POP(vm);     // pop return value from top of the stack
                vm->sp = vm->fp;    // ... return from procedure address ...
                vm->pc = POP(vm);   // ... restore instruction pointer ...
                vm->fp = POP(vm);   // ... restore framepointer ...
                argc = POP(vm);     // ... hom many args procedure has ...
                vm->sp -= argc;     // ... discard all of the args left ...
                PUSH(vm, 0);     // ... leave return value on top of the stack
                break;
            }
            case I_RET_V:
            {
                rval = POP(vm);     // pop return value from top of the stack
                vm->sp = vm->fp;    // ... return from procedure address ...
                vm->pc = POP(vm);   // ... restore instruction pointer ...
                vm->fp = POP(vm);   // ... restore framepointer ...
                argc = POP(vm);     // ... hom many args procedure has ...
                vm->sp -= argc;     // ... discard all of the args left ...
                PUSH(vm, rval);     // ... leave return value on top of the stack
                break;
            }
            case I_CALL:
            {
                // we expect all args to be on the stack
                addr = NCODE(vm); // get next instruction as an address of procedure jump ...
                argc = NCODE(vm); // ... and next one as number of arguments to load ...
                PUSH(vm, argc);   // ... save num args ...      // push
                PUSH(vm, vm->fp);   // ... save num args ...    // push);
                PUSH(vm, vm->pc);   // ... save num args ...    // push
                vm->fp = vm->sp;  // ... set new frame pointer ...
                vm->pc = addr;    // ... move instruction pointer to target procedure address
                break;
            }
            case I_EXIT:
            {
                running = 0;
                break;
            }
            case I_AND:
            {
                b = POP(vm);
                a = POP(vm);
                a && b ? PUSH(vm, 1) : PUSH(vm, 0);
            }
            case I_OR:
            {
                b = POP(vm);
                a = POP(vm);
                a || b ? PUSH(vm, 1) : PUSH(vm, 0);
            }
            case I_INT_EQ:
            {
                b = POP(vm);
                a = POP(vm);
                a == b ? PUSH(vm, 1) : PUSH(vm, 0);
                break;
            }
            case I_INT_NEQ:
            {
                b = POP(vm);
                a = POP(vm);
                a != b ? PUSH(vm, 1) : PUSH(vm, 0);
                break;
            }
            case I_FLOAT_EQ:
            {
                int slot1 = POP(vm);
                int slot2 = POP(vm);
                Float float1 = vm->float_table[slot1];
                Float float2 = vm->float_table[slot2];
                float1.val == float2.val ? PUSH(vm, 1) : PUSH(vm, 0);
                break;
            }
            case I_FLOAT_NEQ:
            {
                int slot1 = POP(vm);
                int slot2 = POP(vm);
                Float float1 = vm->float_table[slot1];
                Float float2 = vm->float_table[slot2];
                float1.val == float2.val ? PUSH(vm, 0) : PUSH(vm, 1);
                break;
            }
            case I_STRING_EQ:
            {
                int slot1 = POP(vm);
                int slot2 = POP(vm);
                char* str1 = vm->string_table[slot1];
                char* str2 = vm->string_table[slot2];
                strcmp(str1, str2) == 0 ? PUSH(vm, 1) : PUSH(vm, 0);
                break;
            }
            case I_STRING_NEQ:
            {
                int slot1 = POP(vm);
                int slot2 = POP(vm);
                char* str1 = vm->string_table[slot1];
                char* str2 = vm->string_table[slot2];
                strcmp(str1, str2) == 0 ? PUSH(vm, 0) : PUSH(vm, 1);
                break;
            }
            case I_INT_PRINT:
            {
                printf("%d", POP(vm));  // ... and print it
                break;
            }
            case I_FLOAT_PRINT:
            {
                Float f;            // utils.c
                int slot = POP(vm);
                f = vm->float_table[slot];
                printf("%f", f.val);
                break;
            }
            case I_STRING_PRINT:
            {
                int slot = POP(vm);
                printf("%s", vm->string_table[slot]);
                break;
            }
            case I_INT_READ:
            {
                offset = NCODE(vm);
                int val;
                scanf("%d", &val);
                vm->locals[vm->fp+offset] = val;
                break;
            }
            case I_FLOAT_READ:
            {
                offset = NCODE(vm);
                float val;
                Float f;
                scanf("%f", &val);
                f.val = val;
                vm->locals[vm->fp+offset] = add_float(vm, f);
                break;
            }
            case I_STRING_READ:
            {
                offset = NCODE(vm);
                char* val = malloc(sizeof(char)*128);
                scanf("%s", val);
                vm->locals[vm->fp+offset] = add_string(vm, val);
                break;
            }
            case I_NOT:
            {
                v = POP(vm);
                PUSH(vm, !v);
                break;
            }
            default:
            {
                printf("No such instruction!\n");
            }

        }
    }while(running);
    vm_exit(vm, 0);
}
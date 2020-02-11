#ifndef VM_H
#define VM_H
#include "../../utils/utils.h"

// stack will have fixed size
#define STACK_SIZE 512

typedef struct {
	Float float_table[1024];	// for storing float variables		// that's silly!!!
	char* string_table[1024];	// for storing string variables		// that's silly!!!
    int* locals;    			// local scoped data
    int* code;      			// array od byte codes to be executed
    int* stack;     			// virtual stack
    int pc;         			// program counter (aka. IP - instruction pointer)
    int sp;         			// stack pointer
    int fp;         			// frame pointer (for local scope)
} VM;

VM* newVM(int*, int, int);
void delVM(VM*);
void run(VM*);

void PUSH(VM*, int);
int POP(VM*);
int NCODE(VM*);

#endif /* VM_H */
#ifndef CODE_H
#define CODE_H
#include "../../frontend/ast/tree.h"
#include "../../frontend/symbol/symbol.h"
#include "instrs.h"

typedef struct code {
	Instruction instr;
	int offset;
	int* jump_slot;
	union {
		struct {
			int num_args;
			int label;
			Token call_name;
		} call_val;
		int label_val;
		Token op_val;
	} val;
	struct code* next;
} Code;
Code* chead;
Code* ctail;
int offset;

/* adds new code with name to tail */
Code* add_code(int);

void code_expr(EXP_node*, Symbol_table*);
void code_primary_exp(EXP_node*, Symbol_table*);
void code_unary_exp(EXP_node*, Symbol_table*);
void code_binary_exp(EXP_node*, Symbol_table*);

void print_code();
int* get_code();

int stack_slot;
int stack_slot_index;
void clear_stack_slot_index();
int get_next_stack_slot_index();

#endif /* CODE_H */
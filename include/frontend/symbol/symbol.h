#ifndef SYMBOL_H
#define SYMBOL_H
#define hash_size 317
#include "../ast/tree.h"
#include "../lexer/token.h"

typedef enum { 
	param_sym,
	field_sym,
	func_sym,
} Sym_kind;

typedef struct symbol {
	Token key;
	int kind;
	int stack_slot;
	union {
		TYPE_node* var_val;
		struct {
			PARAM_node_list* params;
			TYPE_node* return_type;
		} func_sym_val;
	} val;
	struct symbol* next;
} Symbol;

/* one symbol table per scope */
typedef struct $ {
	Symbol* table[hash_size];
	STMT_node* scope_kind;	// while, for, if, etc...
	struct $* next;
} Symbol_table;
Symbol_table* global_scope;

int Hash(char*);
Symbol_table* init_symbol_table();
Symbol_table* scope_symbol_table(Symbol_table*, STMT_node*);
Symbol_table* unscope_symbol_table(Symbol_table*);
Symbol* put_symbol(Symbol_table*, Token, int);
Symbol* get_symbol(Symbol_table*, Token);
int def_symbol(Symbol_table*, Token);

#endif /* SYMBOL_H */
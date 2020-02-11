#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H
#include "symbol.h"
#include "../../utils/utils.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))		// for type_check_invoke()

/* type checking possible values */
enum type_check_enum {
	TYPE_KIND_MISMATCH,					// int* = int, int** = int*
	TYPE_MISMATCH,						// int = float, int* = float*
	VOID_TYPE_USE,						// void + 5, void == void
	TYPE_MATCH,							// int = int, float*** = float***
};
/* returns one of the possible values above for given types */
int check_types(TYPE_node*, TYPE_node*);

/* primitive types */
TYPE_node* BOOL;
TYPE_node* INT;
TYPE_node* VOID;
TYPE_node* FLOAT;
TYPE_node* STRING;
void init_primitive_types();

/* pointer types */
TYPE_node* BOOL_PTR;
TYPE_node* INT_PTR;
TYPE_node* VOID_PTR;
TYPE_node* FLOAT_PTR;
TYPE_node* STRING_PTR;
void init_pointer_types();

TYPE_node* get_var_type(Token, Symbol_table*);
TYPE_node* get_binary_type(EXP_node*, Symbol_table*);
TYPE_node* get_expr_type(EXP_node*, Symbol_table*);
TYPE_node* type_check_invoke(Token, EXP_node_list*, Symbol_table*);
TYPE_node* get_unary_type(EXP_node*, Symbol_table*);
PARAM_node_list* get_function_params(Token, Symbol_table*);
TYPE_node* get_function_type(Token, Symbol_table*);
void type_check_var_decl(Symbol_table*, TYPE_node*, EXP_node*);
void type_check_return(Symbol_table*, EXP_node*, Token);
void type_check_input(Token, Symbol_table*);
Symbol* get_sym(Symbol_table*, Token);

#endif /* TYPE_CHECKER_H */
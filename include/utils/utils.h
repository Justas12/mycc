#ifndef UTILS_H
#define UTILS_H
#include "../frontend/symbol/type_checker.h"
#include "../backend/code/code.h"
#include "../backend/code/instrs.h"

/*-----------------------COMMON-----------------------------*/
/* filename of current file */
char* filename;

/* error indicator */
int error;

/* float to bytes and vice versa */
typedef union f {
	char bytes[4];
	float val;
} Float;

/* err logger */
void lerr(const char*, int);
void perr(const char*);

/* returns single quoted string of given string */
char* str_quote(const char*);

/* frees occupied memory */ 
void free_memory(void*);

/*-----------------------COMMON-----------------------------*/

/*-------------------------------------------------AST HELPERS-------------------------------------------------------*/
/* returns operator token of given expression for line no. access */
Token get_expr_token(EXP_node*);

/* returns string representation of given type */	// (IntType) -> "int"; (PointerType => IntType) -> "int*"
char* str_type(TYPE_node*);

/* returns string representation for operator of given expression */	// OP_LOR -> "||"; OP_MULEQ -> "*="
char* str_operator(EXP_node*);

/* 1 if statement list has a statement which is of specified kind, 0 otherwise */
int has_stmt(STMT_node_list*, int);

/* returns type kind for primary type, -1 if it is a poiner type */
int get_type_kind(TYPE_node*);

/* returns kind of a given expression */
int get_expr_kind(EXP_node*);

/* returns number of expressions in a list */
int get_exprs_count(EXP_node_list*);

/* returns number of parameters in a list */
int get_params_count(PARAM_node_list*);

/* returns a pointer to FNDECL_node by provided key if found, NULL otherwise */
FNDECL_node* get_function(Token, FUNC_node_list*);
/*-------------------------------------------------AST HELPERS-------------------------------------------------------*/

/*-----------------------OTHRER-----------------------------*/
/* copy value that was previously saved for jumps */
void copy_jump_to_label(Code*, int);

/* returns constructed 'LIT_INT' token with provided value and line no.  */
Token make_int_token(int, int);

/* returns constructed 'LIT_FLOAT' token with provided value and line no.  */
Token make_float_token(float, int);

/* returns constructed any other type token with provided value and line no.  */
Token make_token(int, char*, int);

/* constructs error report message */
void _report_err(const char*, const char*, TYPE_node*, TYPE_node*, Token);
/*-----------------------OTHRER-----------------------------*/

#endif /* UTILS_H */
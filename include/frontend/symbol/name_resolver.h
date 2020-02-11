#ifndef NAME_RESOLVER_H
#define NAME_RESOLVER_H
#include "../symbol/symbol.h"

void report_err(const char*, Token, const char*);
void sym_var_decl(Symbol_table*, TYPE_node*, Token, EXP_node*);
void sym_var_assign(Symbol_table*, Token);
void sym_input(Symbol_table*, Token*);
void sym_exprs(Symbol_table*, EXP_node_list*);
void sym_expr(Symbol_table*, EXP_node*);
void sym_output(Symbol_table*, EXP_node_list*);
void sym_block(Symbol_table*, STMT_node_list*, STMT_node*, Token, int, int*);
void sym_loop_for(Symbol_table*, EXP_node*, EXP_node*, EXP_node*, STMT_node_list*, STMT_node*, Token);
void sym_loop_for_in(Symbol_table*, Token, EXP_node_list*, STMT_node_list*, STMT_node*, Token);
void sym_while(Symbol_table*, EXP_node*, STMT_node_list*, STMT_node*, Token);
void sym_do_while(Symbol_table*, EXP_node*, STMT_node_list*, STMT_node*, Token);
void sym_if(Symbol_table*, BRANCH_node_list*, STMT_node_list*, STMT_node*, Token, int, int*);
void sym_return(Symbol_table*, EXP_node*, Token);
void sym_stmt(Symbol_table*, STMT_node*, Token, int, int*);
void sym_param(Symbol_table*, PARAM_node*, int);
void sym_function(FNDECL_node*);
int check_main_signature();
Symbol_table* sym_function_body(FNDECL_node*);
Symbol_table** sym_program(PROGRAM_node*);

#endif /* NAME_RESOLVER_H */
#ifndef TREE_PRINTER_H
#define TREE_PRINTER_H
#include "tree.h"

void print_type(TYPE_node*, int);
void print_expr(EXP_node*, int);
void print_unary_expr(EXP_node*, int);
void print_binary_expr(EXP_node*, int);
void print_primary_expr(EXP_node*, int);
void print_stmt(STMT_node*, int);
void print_param(PARAM_node*);
void print_function(FNDECL_node*);
void print_program(PROGRAM_node*);

#endif /* TREE_PRINTER_H */
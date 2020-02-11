#ifndef PARSER_H
#define PARSER_H
#include "../lexer/token.h"
#include "../ast/tree.h"

void print_expr(EXP_node*, int);
void print_unary_expr(EXP_node*, int);
void print_binary_expr(EXP_node*, int);
void print_primary_expr(EXP_node*, int);
EXP_node_list* parse_expressions();
STMT_node* parse_stmt();
void print_curr_token();
int get_next_token_type();
Token* get_next_token();
void expect(int);
TYPE_node* parse_type();
Token parse_name();
EXP_node* parse_expr_paren();
EXP_node* parse_primary_expr();
EXP_node* parse_unary_expr();
EXP_node* parse_multiplicative_expr();
EXP_node* parse_additive_expr();
EXP_node* parse_relational_expr();
EXP_node* parse_equality_expr();
EXP_node* parse_logical_expr();
EXP_node* parse_expression();
EXP_node_list* parse_expressions();
Token parse_assign_operator();
Token* parse_names();
PARAM_node* parse_param();
STMT_node_list* parse_stmts();
STMT_node* parse_stmt();
PARAM_node_list* parse_params();
FNDECL_node* parse_function();
FUNC_node_list* parse_functions();
PROGRAM_node* parse_program(Token*);

#endif /* PARSER_H */
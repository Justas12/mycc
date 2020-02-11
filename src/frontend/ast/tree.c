#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../include/frontend/ast/tree.h"

/************ CREATE PRIMARY TYPE ********************/
TYPE_node* make_primary_type(int type_kind, Token value) {
	TYPE_node* t = malloc(sizeof(TYPE_node));
	t->kind = primary_type;
	t->val.primary_type.primary_type_kind = type_kind;
	t->val.primary_type.primary_type_value = value;
	return t;
}

/************ CREATE POINTER TYPE ********************/
TYPE_node* make_pointer_type(TYPE_node* node) {
	TYPE_node* t = malloc(sizeof(TYPE_node));
	t->kind = pointer_type;
	t->val.pointer_type_value = node;
	return t;
}

/************ ALLOCATE NEW LINKED LIST NODE WITH PROVIDED FUNCTION ********************/
FUNC_node_list* add_func_to_list(FNDECL_node* node) {
	FUNC_node_list* n = malloc(sizeof(FUNC_node_list));
	n->elem = node;
	n->next = NULL;
	return n;
}

/************ CREATE NEW FUNCTION ********************/
FNDECL_node* make_func(Token name,
					   PARAM_node_list* params,
					   TYPE_node* ret_type,
					   STMT_node_list* body) {
	FNDECL_node* n = malloc(sizeof(FNDECL_node));
	n->func_name = name;
	n->params = params;
	n->ret_type = ret_type;
	n->body = body;
	return n;
}

/************ ALLOCATE NEW LINKED LIST node WITH PROVIDED PARAM ********************/
PARAM_node_list* add_param_to_list(PARAM_node* node) {
	PARAM_node_list* p = malloc(sizeof(PARAM_node_list));
	p->elem = node;
	p->next = NULL;
	return p;
}

/************ CREATE NEW PARAM ********************/
PARAM_node* make_param(Token name, TYPE_node* type) {
	PARAM_node* p = malloc(sizeof(PARAM_node));
	p->name = name;
	p->type = type;
	return p;
}

/************ ALLOCATE NEW LINKED LIST node WITH PROVIDED STMT ********************/
STMT_node_list* add_stmt_to_list(STMT_node* node) {
	STMT_node_list* p = malloc(sizeof(STMT_node_list));
	p->elem = node;
	p->next = NULL;
	return p;
}

/************ CREATE BRANCH ********************/
BRANCH_node* make_branch(EXP_node* cond, STMT_node_list* body) {
	BRANCH_node* b = malloc(sizeof(BRANCH_node));
	b->cond = cond;
	b->body = body;
	return b;
}

/************ ALLOCATE NEW LINKED LIST node WITH PROVIDED BRANCH ********************/
BRANCH_node_list* add_branch_to_list(BRANCH_node* node) {
	BRANCH_node_list* p = malloc(sizeof(BRANCH_node_list));
	p->elem = node;
	p->next = NULL;
	return p;
}

/************ CREATE IF STMT ********************/
STMT_node* make_if_stmt(BRANCH_node_list* branches, STMT_node_list* else_body) {
	STMT_node* p = malloc(sizeof(STMT_node));
	p->kind = statement_if;
	p->val.stmtIf.branches = branches;
	p->val.stmtIf.else_body = else_body;
	return p;
}

/************ CREATE FOR STMT ********************/
STMT_node* make_for_stmt(EXP_node* init, EXP_node* cond, EXP_node* inc, STMT_node_list* body) {
	STMT_node* p = malloc(sizeof(STMT_node));
	p->kind = loop_for;
	p->val.loopFor.init = init;
	p->val.loopFor.cond = cond;
	p->val.loopFor.inc = inc;
	p->val.loopFor.body = body;
	return p;
}

/************ CREATE BREAK STMT ********************/
STMT_node* make_break_stmt(Token t) {
	STMT_node* p = malloc(sizeof(STMT_node));
	p->kind = break_stmt;
	p->val.stmtBreak.lineno = t;
	return p;
}

/************ CREATE CONTINUE STMT ********************/
STMT_node* make_continue_stmt(Token t) {
	STMT_node* p = malloc(sizeof(STMT_node));
	p->kind = continue_stmt;
	p->val.stmtContinue.lineno = t;
	return p;
}

/************ CREATE FOR-IN STMT ********************/
STMT_node* make_for_in_stmt(Token name, EXP_node_list* expressions, STMT_node_list* body) {
	STMT_node* p = malloc(sizeof(STMT_node));
	p->kind = loop_for_in;
	p->val.loopForIn.name = name;
	p->val.loopForIn.expressions = expressions;
	p->val.loopForIn.body = body;
	return p;
}

/************ CREATE WHILE STMT ********************/
STMT_node* make_while_stmt(EXP_node* expression, STMT_node_list* body) {
	STMT_node* p = malloc(sizeof(STMT_node));
	p->kind = loop_while;
	p->val.loopWhile.cond = expression;
	p->val.loopWhile.body = body;
	return p;
}
/************ CREATE DO-WHILE STMT ********************/
STMT_node* make_do_while_stmt(EXP_node* expression, STMT_node_list* body) {
	STMT_node* p = malloc(sizeof(STMT_node));
	p->kind = loop_do_while;
	p->val.loopDoWhile.cond = expression;
	p->val.loopDoWhile.body = body;
	return p;
}

/************ CREATE RETURN STMT ********************/
STMT_node* make_return_stmt(EXP_node* exp) {
	STMT_node* p = malloc(sizeof(STMT_node));
	p->kind = return_stmt;
	p->val.stmtReturn.expression = exp;
	return p;
}

/************ CREATE VAR DECLARATION STMT ********************/
STMT_node* make_var_decl_stmt(TYPE_node* type, Token name, EXP_node* exp) {
	STMT_node* p = malloc(sizeof(STMT_node));
	p->kind = var_decl;
	p->val.varDecl.type = type;
	p->val.varDecl.name = name;
	p->val.varDecl.decl_val = exp;
	return p;
}

/************ CREATE INPUT STMT ********************/
STMT_node* make_input_stmt(Token* idents) {
	STMT_node* p = malloc(sizeof(STMT_node));
	p->kind = input;
	p->val.inputStmt.idents = idents;
	return p;
}

/************ CREATE OUTPUT STMT ********************/
STMT_node* make_output_stmt(EXP_node_list* expressions) {
	STMT_node* p = malloc(sizeof(STMT_node));
	p->kind = output;
	p->val.outputStmt.expressions = expressions;
	return p;
}

/************ CREATE EXPRESSION STMT ********************/
STMT_node* make_expr_stmt(EXP_node* exp) {
	STMT_node* p = malloc(sizeof(STMT_node));
	p->kind = expr_stmt;
	p->val.exprStmt.expression = exp;
	return p;
}

/************ ALLOCATE NEW LINKED LIST node WITH PROVIDED EXPR ********************/
EXP_node_list* add_expr_to_list(EXP_node* node) {
	EXP_node_list* p = malloc(sizeof(EXP_node_list));
	p->elem = node;
	p->next = NULL;
	return p;
}

/************ CREATE PRIMARY CONSTANT ********************/
EXP_node* make_primary_const(Token token, int kind) {
  EXP_node* e = malloc(sizeof(EXP_node));
  e->kind = primary_exp;
  e->val.primaryExp.primary_exp_kind = kind;
  e->val.primaryExp.primary_exp_val.constantExp = token;
  return e;
};

/************ CREATE PRIMARY CALL ********************/
EXP_node* make_primary_call(Token name, EXP_node_list* args) {
	EXP_node* e = malloc(sizeof(EXP_node));
	e->kind = primary_exp;
	e->val.primaryExp.primary_exp_kind = call_exp;
	e->val.primaryExp.primary_exp_val.callExp.name = name;
	e->val.primaryExp.primary_exp_val.callExp.arguments = args;
	return e;
}

/************ CREATE BINARY EXPRESSION ********************/
EXP_node* make_binary_exp(int kind, Token operator, EXP_node* left, EXP_node* right) {
  EXP_node* e = malloc(sizeof(EXP_node));
  e->kind = binary_exp;
  e->val.binaryExp.binary_exp_kind = kind;
  e->val.binaryExp.operator = operator;
  e->val.binaryExp.left = left;
  e->val.binaryExp.right = right;
  return e;
};

/************ CREATE UNARY EXPRESSION ********************/
EXP_node* make_unary_exp(int kind, Token operator, EXP_node* operand) {
  EXP_node* e = malloc(sizeof(EXP_node));
  e->kind = unary_exp;
  e->val.unaryExp.unary_exp_kind = kind;
  e->val.unaryExp.operator = operator;
  e->val.unaryExp.operand = operand;
  return e;
};

/************ CREATE PROGRAM NODE ********************/
PROGRAM_node* make_program_node(const char* s) {
  PROGRAM_node* p = malloc(sizeof(PROGRAM_node));
  Token t;
  t.value.str_val = malloc(sizeof(char)*strlen(s)+1);
  strcpy(t.value.str_val, s);
  p->name = t;
  return p;
};
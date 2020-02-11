#include <string.h>
#include <stdlib.h>
#include "../../include/utils/utils.h"

void lerr(const char* msg, int lineno) {
	fprintf(stderr, "%s:%d error: %s\n", filename,
											 lineno,
											 msg);
	// exit(1);
	error = 1;
}

void perr(const char* msg) {
	perror(msg);
	exit(1);
}

char* str_quote(const char* s) {
	int sz = strlen(s) + 3;
	char* str = malloc(sizeof(char)*sz);
	strcpy(str, "'");
	strcat(str, s);
	strcat(str, "'");
	return str;
}

void free_memory(void* mem) {
	free(mem);
}

char* str_type(TYPE_node* t) {
	char* str = malloc(sizeof(char)*20);
	switch(t->kind) {
		case primary_type:
		{
			switch(t->val.primary_type.primary_type_kind) {
				case IntType:
				{
					strcpy(str, "int");
					break;
				}
				case BooleanType:
				{
					strcpy(str, "boolean");
					break;
				}
				case StringType:
				{
					strcpy(str, "string");
					break;
				}
				case FloatType:
				{
					strcpy(str, "float");
					break;
				}
				case VoidType:
				{
					strcpy(str, "void");
					break;
				}
				default:
				{
					lerr("Unknown primary type!", -1);
				}
			}
			break;
		}
		case pointer_type:
		{
			str = str_type(t->val.pointer_type_value);
			strcat(str, "*");
			break;
		}
		default:
		{
			lerr("Unknown type kind!", -1);
		}
	}
	return str;
}

Token get_expr_token(EXP_node* e) {
	Token t;
	switch(e->kind) {
		case primary_exp:
		{
			if (e->val.primaryExp.primary_exp_kind == call_exp)
				t = e->val.primaryExp.primary_exp_val.callExp.name;
			else
				t = e->val.primaryExp.primary_exp_val.constantExp;
			break;
		}
		case binary_exp:
		{
			t = e->val.binaryExp.operator;
			break;
		}
		case unary_exp:
		{
			t = e->val.unaryExp.operator;
			break;
		}
		default:
		{
			lerr("Unknown expression kind!", -1);
		}
	}
	return t;
}

char* str_operator(EXP_node* e) {
	char* str = malloc(sizeof(char)*5);
	if (e->kind == binary_exp) {
		switch(e->val.binaryExp.operator.token_type) {
			case OP_LS: { strcpy(str, "<"); break; }
			case OP_GR: { strcpy(str, ">"); break; }
			case OP_LSE: { strcpy(str, "<="); break; }
			case OP_GRE: { strcpy(str, ">="); break; }
			case OP_NEQ: { strcpy(str, "!="); break; }
			case OP_AEQ: { strcpy(str, "="); break; }
			case OP_CEQ: { strcpy(str, "=="); break; }
			case OP_LAND: { strcpy(str, "&&"); break; }
			case OP_LOR: { strcpy(str, "||"); break; }
			case OP_DIV: { strcpy(str, "/"); break; }
			case OP_PLUS: { strcpy(str, "+"); break; }
			case OP_MINUS: { strcpy(str, "-"); break; }
			case OP_MULT: { strcpy(str, "*"); break; }
			case OP_PERC: { strcpy(str, "%"); break; }
			case OP_MULEQ: { strcpy(str, "*="); break; }
			case OP_PLEQ: { strcpy(str, "+="); break; }
			case OP_MINEQ: { strcpy(str, "-="); break; }
			case OP_DIVEQ: { strcpy(str, "/="); break; }	
		}
	}
	else if (e->kind == unary_exp) {
		if (e->val.unaryExp.operator.token_type == OP_NT)
			strcpy(str, "!");
		else if (e->val.unaryExp.operator.token_type == OP_MINUS)
			strcpy(str, "-");
		else if (e->val.unaryExp.operator.token_type == OP_BAND)
			strcpy(str, "&");
		else
			strcpy(str, "*");
	}
	else
		str = NULL;
	return str;
}

int has_stmt(STMT_node_list* s, int x) {
	STMT_node_list* head = s;
	int res = 0;
	while (head != NULL) {
		if (head->elem->kind == x)
			return 1;
		else {
			switch(head->elem->kind) {
				case loop_for:
				{
					STMT_node_list* body = head->elem->val.loopFor.body;
					res = has_stmt(body, x);
					break;
				}
				case loop_for_in:
				{
					STMT_node_list* body = head->elem->val.loopForIn.body;
					res = has_stmt(body, x);
					break;
				}
				case loop_while:
				{
					STMT_node_list* body = head->elem->val.loopWhile.body;
					res = has_stmt(body, x);
					break;
				}
				case loop_do_while:
				{
					STMT_node_list* body = head->elem->val.loopDoWhile.body;
					res = has_stmt(body, x);
					break;
				}
				case statement_if:
				{
					BRANCH_node_list* branches = head->elem->val.stmtIf.branches;
					while (branches != NULL) {
						res = has_stmt(branches->elem->body, x);
						if (res == 1)
							return res;
						branches = branches->next;
					}
					if (head->elem->val.stmtIf.else_body != NULL) {
						res = has_stmt(head->elem->val.stmtIf.else_body, x);
						if (res == 1)
							return res;
					}
					break;
				}
				default:
				{
					break;
				}
			}
		}
		head = head->next;
	}
	return res;
}

int get_params_count(PARAM_node_list* params) {
	PARAM_node_list* head = params;
	int i = 0;
	while(head != NULL) {
		head = head->next;
		i++;
	}
	return i;
}

int get_exprs_count(EXP_node_list* args) {
	EXP_node_list* head = args;
	int i = 0;
	while(head != NULL) {
		head = head->next;
		i++;
	}
	return i;
}

FNDECL_node* get_function(Token key, FUNC_node_list* funcs) {
	FUNC_node_list* head = funcs;
	while (head != NULL) {
		if (strcmp(head->elem->func_name.value.str_val, key.value.str_val) == 0)
			return head->elem;
		head = head->next;
	}
	return NULL;
}

int get_type_kind(TYPE_node* t) {
	if (t->kind == primary_type)
		return t->val.primary_type.primary_type_kind;
	return -1;	// pointer type
}

int get_expr_kind(EXP_node* e) {
	if (e->kind == binary_exp)
		return e->val.binaryExp.binary_exp_kind;
	else if (e->kind == unary_exp)
		return e->val.unaryExp.unary_exp_kind;
	else
		return e->val.primaryExp.primary_exp_kind;
}

void _report_err(const char* msg, const char* op, TYPE_node* a, TYPE_node* b, Token key) {
	int lineno = key.line_no;
	char buffer[256];
	strcpy(buffer, msg);
	if (op != NULL)
		strcat(buffer, str_quote(op));
	if (a != NULL) {
		strcat(buffer, ": ");
		strcat(buffer, str_quote(str_type(a)));
		if (b != NULL) {
			strcat(buffer, " and ");
			strcat(buffer, str_quote(str_type(b)));
		}
	}
	lerr(buffer, lineno);
}

void copy_jump_to_label(Code* start, int val) {
	Code* head = start->next;
	while(head != NULL) {
		if (head->jump_slot != NULL && *(head->jump_slot) == val) {
			head->val.label_val = *(head->jump_slot);
			head->jump_slot = NULL;
		}
		head = head->next;
	}
}

Token make_int_token(int val, int lineno) {
	Token t;
	t.token_type = LIT_INT;
	t.value.int_val = val;
	t.line_no = lineno;
	return t;
}

Token make_float_token(float val, int lineno) {
	Token t;
	t.token_type = LIT_FLOAT;
	t.value.float_val = val;
	t.line_no = lineno;
	return t;
}

Token make_token(int type, char* val, int lineno) {
	Token t;
	t.token_type = type;
	if (val != NULL) {
		t.value.str_val = malloc(strlen(val)*sizeof(char)+1);
		strcpy(t.value.str_val, val);
	}
	t.line_no = lineno;
	return t;
}
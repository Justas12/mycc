#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../../include/frontend/symbol/name_resolver.h"
#include "../../../include/frontend/symbol/type_checker.h"

#include "../../../include/backend/code/code.h"

#include "../../../include/utils/utils.h"

void clear_stack_slot_index() {
	stack_slot_index = 0;
}

int get_next_stack_slot_index() {
	return stack_slot_index + 1;
}

void report_err(const char* msg, Token key, const char* msg2) {
	int lineno = key.line_no;
	char* name = key.value.str_val;
	char buffer[100];

	strcpy(buffer, msg);
	strcat(buffer, " '");
	strcat(buffer, name);
	strcat(buffer, "' ");
	strcat(buffer, msg2);

	lerr(buffer, lineno);
}

Symbol* sym_var(Symbol_table* t, Token key) {
	Symbol* s;
	if ((s = get_symbol(t, key)) == NULL) {
		if ((s = get_symbol(global_scope, key)) == NULL)
			report_err("variable", key, "undeclared!");
	}
	return s;
}

void sym_var_decl(Symbol_table* t, TYPE_node* type, Token key, EXP_node* exp) {
	Symbol* s;
	if (def_symbol(t, key))
		report_err("variable", key, "already declared!");
	s = put_symbol(t, key, field_sym);
	s->val.var_val = type;
	s->stack_slot = stack_slot_index;
	type_check_var_decl(t, type, exp);
	if (exp != NULL) {
		TYPE_node* exp_type = get_expr_type(exp, t);		// fix ???
		if (exp_type != NULL) {
			code_expr(exp, t);
			Code* c = add_code(I_SET_L);
			c->val.label_val = s->stack_slot;
		}
	}
}

void sym_input(Symbol_table* t, Token* keys) {
	int i;
	for (i = 0; keys[i].value.str_val != NULL; ++i) {
		Symbol* s = get_symbol(t, keys[i]);
		if (s != NULL && s->kind != func_sym) {
			switch(s->val.var_val->kind) {
				case primary_type:
				{
					Code* c;
					switch(s->val.var_val->val.primary_type.primary_type_kind) {
						case IntType:
						{
							c = add_code(I_INT_READ);
							break;
						}
						case BooleanType:
						{
							c = add_code(I_INT_READ);
							break;
						}
						case StringType:
						{
							c = add_code(I_STRING_READ);
							break;
						}
						case FloatType:
						{
							c = add_code(I_FLOAT_READ);
							break;
						}
						case VoidType:
						{
							printf("Void type not allowed here!\n");
							break;
						}
						default:
						{
							lerr("Unknown primary type!", -1);
						}
					}
					c->val.label_val = s->stack_slot;
					break;
				}
				case pointer_type:
				{
					printf("Pointer type not allowed here!\n");
					break;
				}
				default:
				{
					lerr("Unknown type kind!", -1);
				}
			}
		}
	}
}

void sym_output(Symbol_table* t, EXP_node_list* exprs) {
	EXP_node_list* head = exprs;
	while (head != NULL) {
		code_expr(head->elem, t);
		TYPE_node* type = get_expr_type(head->elem, t);
		if (type != NULL) {
			switch(type->kind) {
				case primary_type:
				{
					switch(type->val.primary_type.primary_type_kind) {
						case IntType:
						{
							add_code(I_INT_PRINT);
							break;
						}
						case BooleanType:
						{
							add_code(I_INT_PRINT);
							break;
						}
						case StringType:
						{
							add_code(I_STRING_PRINT);
							break;
						}
						case FloatType:
						{
							add_code(I_FLOAT_PRINT);
							break;
						}
						case VoidType:
						{
							printf("Void type not allowed here!\n");
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
					printf("Pointer type not allowed here!\n");
					break;
				}
				default:
				{
					lerr("Unknown type kind!", -1);
				}
			}
		}
		head = head->next;
	}
}

void sym_block(Symbol_table* t, STMT_node_list* body, STMT_node* kind, Token name, int loop_start, int* loop_end) {
	Symbol_table* scp = scope_symbol_table(t, kind);
	STMT_node_list* head = body;
	while(head != NULL) {
		sym_stmt(scp, head->elem, name, loop_start, loop_end);
		head = head->next;
	}
	t = unscope_symbol_table(scp);
}

void sym_loop_for(Symbol_table* t,
						EXP_node* init,
						EXP_node* cond,
						EXP_node* inc,
						STMT_node_list* body,
						STMT_node* s,
						Token name) {
	TYPE_node* cond_type;
	Code* BR;
	Code* BZ;
	int start;
	int end = -1;
	if (init != NULL) {
		get_expr_type(init, t);
		code_expr(init, t);
	}
	start = offset;
	if (cond != NULL) {
		cond_type = get_expr_type(cond, t);
		if (cond_type != NULL) {
			if (check_types(cond_type, BOOL) != TYPE_MATCH) {
				Token token = get_expr_token(cond);
				_report_err("for loop condition must be of type boolean, got", NULL, cond_type, NULL, token);
			}
			code_expr(cond, t);
		}
	}
	else {
		Code* c = add_code(I_INT_PUSH);
		Token t = make_int_token(1, 0);
		c->val.op_val = t;
	}
	BZ = add_code(I_BZ);
 	sym_block(t, body, s, name, start, &end);
	if (inc != NULL) {
		get_expr_type(inc, t);
		code_expr(inc, t);
	}
	BR = add_code(I_BR);
	end = offset;
	BZ->val.label_val = offset;
	BR->val.label_val = start;
	copy_jump_to_label(BZ, end);
}

void sym_loop_for_in(Symbol_table* t,
					 Token key,
					 EXP_node_list* exprs,
					 STMT_node_list* body,
					 STMT_node* stmt,
					 Token name) {
	int start;
	int end = -1;
	TYPE_node* var_type = get_var_type(key, t);
	EXP_node_list* head = exprs;
	Symbol* s = get_symbol(t, key);
	Code* START = add_code(I_BR);
	START->val.label_val = offset;
	if (s != NULL) {
		while (head != NULL) {
			Code* c;
			TYPE_node* expr_type = get_expr_type(head->elem, t);
			if (expr_type != NULL && var_type != NULL) {
				if (check_types(var_type, expr_type) != TYPE_MATCH)
					_report_err("list item mismatched variable type ", NULL, var_type, expr_type, key);
				else {
					start = offset;
					code_expr(head->elem, t);
					c = add_code(I_SET_L);
					c->val.label_val = s->stack_slot;
					sym_block(t, body, stmt, name, start, &end);
				}
			}
			head = head->next;
		}
	}
	end = offset;
	copy_jump_to_label(START, end);
}

void sym_while(Symbol_table* t, EXP_node* cond, STMT_node_list* body, STMT_node* s, Token name) {
	TYPE_node* cond_type = get_expr_type(cond, t);
	if (cond_type != NULL) {
		Code* BR;
		Code* BZ;
		int start;
		int end = -1;
		if (check_types(cond_type, BOOL) != TYPE_MATCH) {
			Token token = get_expr_token(cond);
			_report_err("while condition must be of type boolean, got", NULL, cond_type, NULL, token);
		}
		start = offset;
		code_expr(cond, t);
		BZ = add_code(I_BZ);
		sym_block(t, body, s, name, start, &end);
		BR = add_code(I_BR);
		end = offset;
		BZ->val.label_val = offset;
		BR->val.label_val = start;
		copy_jump_to_label(BZ, end);
	}
}

void sym_do_while(Symbol_table* t, EXP_node* cond, STMT_node_list* body, STMT_node* s, Token name) {
	int start = offset;
	int end = -1;
	Code* BR;
	Code* BZ;
	Code* START = add_code(I_BR);
	START->val.label_val = offset;
	sym_block(t, body, s, name, start, &end);
	TYPE_node* cond_type = get_expr_type(cond, t);
	code_expr(cond, t);
	BZ = add_code(I_BZ);
	BR = add_code(I_BR);
	BR->val.label_val = start;
	end = offset;
	BZ->val.label_val = offset;
	if (cond_type != NULL) {
		if (check_types(cond_type, BOOL) != TYPE_MATCH) {
			Token token = get_expr_token(cond);
			_report_err("do-while condition must be of type boolean, got", NULL, cond_type, NULL, token);
		}
	}
	copy_jump_to_label(START, end);
}

void sym_if(Symbol_table* t, BRANCH_node_list* branches, STMT_node_list* else_body, STMT_node* s, Token name, int loop_start, int* loop_end) {
	BRANCH_node_list* head = branches;
	STMT_node_list* body;
	int end = -2;
	Code* START = add_code(I_BR);
	START->val.label_val = offset;
	while (head != NULL) {
		TYPE_node* cond_type = get_expr_type(head->elem->cond, t);
		if (cond_type != NULL) {
			Code* BZ;
			Code* BR;
			if (check_types(cond_type, BOOL) != TYPE_MATCH) {
				Token token = get_expr_token(head->elem->cond);
				_report_err("if condition must be of type boolean, got", NULL, cond_type, NULL, token);
			}
			code_expr(head->elem->cond, t);
			BZ = add_code(I_BZ);
			body = head->elem->body;
			sym_block(t, body, s, name, loop_start, loop_end);
			BR = add_code(I_BR);
			BZ->val.label_val = offset;
			BR->jump_slot = &end;
		}
		head = head->next;
	}
	if (else_body != NULL)
		sym_block(t, else_body, s, name, loop_start, loop_end);
	end = offset;
	copy_jump_to_label(START, end);
}

void sym_return(Symbol_table* t, EXP_node* exp, Token name) {
	type_check_return(t, exp, name);
	if (exp != NULL) {
		code_expr(exp, t);
		add_code(I_RET_V);
	}
	else
		add_code(I_RET);
}

void is_in_loop(Symbol_table* t, STMT_node* s) {
	Symbol_table* head = t;
	int scope;
	while (head->scope_kind != NULL) {
		scope = head->scope_kind->kind;
		if (scope == loop_for 	 ||
			scope == loop_for_in ||
			scope == loop_while  ||
			scope == loop_do_while)
		return;
		head = head->next;
	}
	if (s->kind == continue_stmt)
		_report_err("continue statement not within loop", NULL, NULL, NULL, s->val.stmtContinue.lineno);
	else
		_report_err("break statement not within loop", NULL, NULL, NULL, s->val.stmtBreak.lineno);
}

void sym_stmt(Symbol_table* t, STMT_node* s, Token name, int loop_start, int* loop_end) {
	switch(s->kind) {
		case var_decl:
		{
			sym_var_decl(t, s->val.varDecl.type, s->val.varDecl.name, s->val.varDecl.decl_val);
			stack_slot_index = get_next_stack_slot_index();
			break;
		}
		case input:
		{
			sym_input(t, s->val.inputStmt.idents);
			break;
		}
		case output:
		{
			sym_output(t, s->val.outputStmt.expressions);
			break;
		}
		case loop_for:
		{
			sym_loop_for(t, s->val.loopFor.init,
							 s->val.loopFor.cond,
							 s->val.loopFor.inc,
							 s->val.loopFor.body,
							 s, name);
			break;
		}
		case loop_for_in:
		{
			sym_loop_for_in(t, s->val.loopForIn.name, 
								s->val.loopForIn.expressions, 
								s->val.loopForIn.body,
								s, name);
			break;
		}
		case loop_while:
		{
			sym_while(t, s->val.loopWhile.cond, s->val.loopWhile.body, s, name);
			break;
		}
		case loop_do_while:
		{
			sym_do_while(t, s->val.loopDoWhile.cond, s->val.loopDoWhile.body, s, name);
			break;
		}
		case statement_if:
		{
			sym_if(t, s->val.stmtIf.branches, s->val.stmtIf.else_body, s, name, loop_start, loop_end);
			break;
		}
		case continue_stmt:
		{
			is_in_loop(t, s);
			Code* c = add_code(I_BR);
			c->val.label_val = loop_start;
			break;
		}
		case break_stmt:
		{
			is_in_loop(t, s);
			Code* c = add_code(I_BR);
			c->jump_slot = loop_end;
			break;
		}
		case return_stmt:
		{
			sym_return(t, s->val.stmtReturn.expression, name);
			break;
		}
		case expr_stmt:
		{
			if (get_expr_type(s->val.exprStmt.expression, t) != NULL) {
				code_expr(s->val.exprStmt.expression, t);
			}
			break;
		}

	}
}

void sym_param(Symbol_table* t, PARAM_node* p, int x) {
	TYPE_node* type = p->type;
	Token key = p->name;
	Symbol* s;
	if (def_symbol(t, key))
		report_err("param name", key, "already declared!");
	s = put_symbol(t, key, param_sym);
	s->val.var_val = type;
	s->stack_slot = x;
}

Symbol_table* sym_function_body(FNDECL_node* f) {
	Symbol_table* t;
	PARAM_node_list* params = f->params;
	STMT_node_list* stmts = f->body;
	Token key = f->func_name;
	clear_stack_slot_index();
	t = init_symbol_table();
	f->start_label = offset;
	int params_num = get_params_count(params);
	int x = -2 - params_num;	// to fetch args from stack

	while(params != NULL) {
		sym_param(t, params->elem, x++);
		params = params->next;
	}
	while(stmts != NULL) {
		sym_stmt(t, stmts->elem, key, -1, NULL);
		stmts = stmts->next;
	}
	return t;
}

void sym_function(FNDECL_node* f) {
	TYPE_node* ret_type = f->ret_type;
	PARAM_node_list* params = f->params;
	Token key = f->func_name;
	f->start_label = offset;
	Symbol* s = NULL;

	if (def_symbol(global_scope, key))
		report_err("function name", key, "already declared!");
	s = put_symbol(global_scope, key, func_sym);
	s->val.func_sym_val.return_type = ret_type;
	s->val.func_sym_val.params = params;
}

int check_main_signature(Token key) {
	if (def_symbol(global_scope, key) == 0) {
		report_err("function", key, "undeclared");
		return -1;
	}
	TYPE_node* type = get_function_type(key, global_scope);
	PARAM_node_list* params = get_function_params(key, global_scope);
	int params_num = get_params_count(params);
	if (params != NULL) {
		if (check_types(INT, type) != TYPE_MATCH)
			lerr("main function must be of type int", key.line_no);
		if (params_num != 0 && params_num != 2)
			lerr("main function parameters must either be () or ('int', 'string*')", key.line_no);
		if (params_num == 2) {
			if (check_types(INT, params->elem->type) != TYPE_MATCH)
				lerr("first parameter of main function must be of type 'int'", key.line_no);
			if (check_types(STRING_PTR, params->next->elem->type) != TYPE_MATCH)
				lerr("second parameter of main function must be of type 'string*'", key.line_no);
		}
	}
	return params_num;
}

Symbol_table** sym_program(PROGRAM_node* p) {
	FUNC_node_list* head = p->functions;
	global_scope = init_symbol_table();
	init_primitive_types();
	init_pointer_types();
	while (head != NULL) {
		sym_function(head->elem);
		head = head->next;
	}
	Token main_key = make_token(0, "main", 0);
	int main_args = check_main_signature(main_key);
	if (main_args == -1)
		return NULL;
	Code* main_call = add_code(I_CALL);
	add_code(I_EXIT);
	main_call->val.call_val.num_args = main_args;
	main_call->val.call_val.call_name = main_key;
	main_call->val.call_val.label = 20;
	head = p->functions;
	while (head != NULL) {
		sym_function_body(head->elem);
		TYPE_node* type = get_function_type(head->elem->func_name, global_scope);
		if (check_types(type, BOOL) != VOID_TYPE_USE) {
			if (has_stmt(head->elem->body, return_stmt) == 0)
				lerr("return statement expected", head->elem->func_name.line_no);
		}
		else if (has_stmt(head->elem->body, return_stmt) == 0)
			add_code(I_RET);
		head = head->next;
	}
	Code* hcode = chead;
	while (hcode != NULL) {
		if (hcode->instr.name == I_CALL) {
			Token key = hcode->val.call_val.call_name;
			FNDECL_node* f = get_function(key, p->functions);
			hcode->val.call_val.label = f->start_label;
		}
		hcode = hcode->next;
	}
	return NULL;
}
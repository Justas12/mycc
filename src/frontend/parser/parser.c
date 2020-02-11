#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include "../../../include/frontend/parser/parser.h"
#include "../../../include/utils/utils.h"

Token* curr_token = NULL;

void print_curr_token() {
	printf("token type -> %s\n", token_type_string[curr_token->token_type]);
}

int get_next_token_type() {
	return (curr_token+1)->token_type;
}

Token* get_next_token() {
	++curr_token;
	return curr_token;
}

void expect(int expected_type) {
	if (curr_token->token_type != expected_type) {
		char s[128];
		sprintf(s, "expected '%s'", token_type_symbol[expected_type]);
		lerr(s, (curr_token-1)->line_no);
		exit(1);
	}
	curr_token = get_next_token();
}

TYPE_node* parse_type() {
	TYPE_node* type = NULL;
	switch(curr_token->token_type) {
		case KW_VOID:
		{
			type = make_primary_type(VoidType, *curr_token);
				while(get_next_token_type() == OP_MULT) {
					type = make_pointer_type(type);
					curr_token = get_next_token();
				}
			break;
		}
		case KW_INT:
		{
			type = make_primary_type(IntType, *curr_token);
				while(get_next_token_type() == OP_MULT) {
					type = make_pointer_type(type);
					curr_token = get_next_token();
				}
			break;
		}
		case KW_FLOAT:
		{
			type = make_primary_type(FloatType, *curr_token);
				while(get_next_token_type() == OP_MULT) {
					type = make_pointer_type(type);
					curr_token = get_next_token();
				}
			break;
		}
		case KW_BOOLEAN:
		{
			type = make_primary_type(BooleanType, *curr_token);
				while(get_next_token_type() == OP_MULT) {
					type = make_pointer_type(type);
					curr_token = get_next_token();
				}
			break;
		}
		case KW_STRING:
		{
			type = make_primary_type(StringType, *curr_token);
				while(get_next_token_type() == OP_MULT) {
					type = make_pointer_type(type);
					curr_token = get_next_token();
				}
			break;
		}
	}
	return type;
}

Token parse_name() {
	Token name;
	if (curr_token->token_type == IDENT)
		name = *curr_token;
	else 
		lerr("variable name not specified!", curr_token->line_no);
	curr_token = get_next_token();
	return name;
}

EXP_node* parse_expr_paren() {
	EXP_node* result = NULL;
	expect(OP_PAREN_OPEN);
	result = parse_expression();
	expect(OP_PAREN_CLOSE);
	return result;
}

EXP_node* parse_primary_expr() {
	EXP_node* result = NULL;
	switch(curr_token->token_type) {
  		case IDENT:
		{
			if (get_next_token_type() == OP_PAREN_OPEN) {
				Token name = *curr_token;
				curr_token = get_next_token();
				curr_token = get_next_token();
				if (curr_token->token_type == OP_PAREN_CLOSE) {
					curr_token = get_next_token();
					return make_primary_call(name, NULL);
				}
				else {
					EXP_node_list* args = parse_expressions();
					result = make_primary_call(name, args);
					expect(OP_PAREN_CLOSE);
					return result;
				}
			}
			else
				result = make_primary_const(*curr_token, variable_exp);
			break;
		}
		case LIT_INT:
		{
			result = make_primary_const(*curr_token, integer_exp);
			break;
		}
		case LIT_STR:
		{
			result = make_primary_const(*curr_token, string_exp);
			break;
		}
		case LIT_FLOAT:
		{
			result = make_primary_const(*curr_token, float_exp);
			break;
		}
		case KW_FALSE:
		{
			result = make_primary_const(*curr_token, boolean_exp);
			break;
		}
		case KW_TRUE:
		{
			result = make_primary_const(*curr_token, boolean_exp);
			break;
		}
		case OP_PAREN_OPEN:
		{
			return parse_expr_paren();
			break;
		}
		default:
		{
			return NULL;
		}
	}
	curr_token = get_next_token();
	return result;
}

EXP_node* parse_unary_expr() {
	EXP_node* result = parse_primary_expr();
	if (result == NULL) {
		while(1) {
			if (curr_token->token_type == OP_MINUS) {
				Token t = *curr_token;
				curr_token = get_next_token();
				EXP_node* e = parse_unary_expr();
				if (e != NULL)
					result = make_unary_exp(inverse_exp, t, e);
				break;
			}
			else if (curr_token->token_type == OP_NT) {
				Token t = *curr_token;
				curr_token = get_next_token();
				EXP_node* e = parse_unary_expr();
				if (e != NULL)
					result = make_unary_exp(negation_exp, t, e);
				break;
			}
			else if (curr_token->token_type == OP_BAND) {
				// NOT IMPLEMENTED
				break;
			}
			else if (curr_token->token_type == OP_MULT) {
				// NOT IMPLEMENTED
				break;
			}
			else
				break;
		}
	}
	return result;
}

EXP_node* parse_multiplicative_expr() {
	EXP_node* result = parse_unary_expr();
	while(1) {
		if (result != NULL) {
			if (curr_token->token_type == OP_MULT) {
				Token t = *curr_token;
				curr_token = get_next_token();
				if (result != NULL) {
					result = make_binary_exp(multiplicative_exp, t, result, parse_unary_expr());
				}
			}
			else if (curr_token->token_type == OP_PERC) {
				Token t = *curr_token;
				curr_token = get_next_token();
				result = make_binary_exp(multiplicative_exp, t, result, parse_unary_expr());
			}
			else if (curr_token->token_type == OP_DIV) {
				Token t = *curr_token;
				curr_token = get_next_token();
				result = make_binary_exp(multiplicative_exp, t, result, parse_unary_expr());
			}
			else
				break;
		}
		else
			break;
	}
	return result;
}

EXP_node* parse_additive_expr() {
	EXP_node* result = parse_multiplicative_expr();
	while(1) {
		if (result != NULL) {
			if (curr_token->token_type == OP_PLUS) {
				Token t = *curr_token;
				curr_token = get_next_token();
				result = make_binary_exp(additive_exp, t, result, parse_multiplicative_expr());
			}
			else if (curr_token->token_type == OP_MINUS) {
				Token t = *curr_token;
				curr_token = get_next_token();
				result = make_binary_exp(additive_exp, t, result, parse_multiplicative_expr());
			}
			else
				break;
		}
		else
			break;
	}
	return result;
}

EXP_node* parse_relational_expr() {
	EXP_node* result = parse_additive_expr();
	while(1) {
		if (result != NULL) {
			if (curr_token->token_type == OP_LS) {
				Token t = *curr_token;
				curr_token = get_next_token();
				result = make_binary_exp(relational_exp, t, result, parse_additive_expr());
			}
			else if (curr_token->token_type == OP_LSE) {
				Token t = *curr_token;
				curr_token = get_next_token();
				result = make_binary_exp(relational_exp, t, result, parse_additive_expr());
			}
			else if (curr_token->token_type == OP_GR) {
				Token t = *curr_token;
				curr_token = get_next_token();
				result = make_binary_exp(relational_exp, t, result, parse_additive_expr());
			}
			else if (curr_token->token_type == OP_GRE) {
				Token t = *curr_token;
				curr_token = get_next_token();
				result = make_binary_exp(relational_exp, t, result, parse_additive_expr());
			}
			else
				break;
		}
		else
			break;
	}
	return result;
}

EXP_node* parse_equality_expr() {
	EXP_node* result = parse_relational_expr();
	while(1) {
		if (result != NULL) {
			if (curr_token->token_type == OP_CEQ) {
				Token t = *curr_token;
				curr_token = get_next_token();
				result = make_binary_exp(equality_exp, t, result, parse_relational_expr());
			}
			else if (curr_token->token_type == OP_NEQ) {
				Token t = *curr_token;
				curr_token = get_next_token();
				result = make_binary_exp(equality_exp, t, result, parse_relational_expr());
			}
			else
				break;
		}
		else
			break;
	}
	return result;
}

EXP_node* parse_logical_expr() {
	EXP_node* result = parse_equality_expr();
	while(1) {
		if (result != NULL) {
			if (curr_token->token_type == OP_LAND) {
				Token t = *curr_token;
				curr_token = get_next_token();
				result = make_binary_exp(logical_exp, t, result, parse_equality_expr());
			}
			else if (curr_token->token_type == OP_LOR) {
				Token t = *curr_token;
				curr_token = get_next_token();
				result = make_binary_exp(logical_exp, t, result, parse_equality_expr());
			}
			else
				break;
		}
		else
			break;
	}
	return result;
}

EXP_node* parse_assign_expr() {
	EXP_node* result = parse_logical_expr();
	while(1) {
		Token t = parse_assign_operator();
		if (t.token_type != -1 && result != NULL) {
			if (result->kind == primary_exp || result->kind == unary_exp) {
				if (result->val.primaryExp.primary_exp_kind == variable_exp || 
					result->val.unaryExp.unary_exp_kind == dereference_exp)
				{
					Token t = *curr_token;
					curr_token = get_next_token();
					result = make_binary_exp(assign_exp, t, result, parse_logical_expr());
				}
				else
					lerr("lvalue required as left operand of assignment", curr_token->line_no);
				break;
			}
			else
				lerr("lvalue required as left operand of assignment", curr_token->line_no);
		}
		else 
			break;
	}
	return result;
}

EXP_node* parse_expression() {
	EXP_node* e = NULL;
	e = parse_assign_expr();
	return e;
}

EXP_node_list* parse_expressions() {
	EXP_node_list* head = NULL;  
	EXP_node_list* tail = NULL;
	EXP_node* e = NULL;
	e = parse_expression();
	if (e != NULL)
		head = add_expr_to_list(e);
	tail = head;
	while(curr_token->token_type == OP_COMMA) {
		curr_token = get_next_token();
		e = parse_expression();
		if (e != NULL) {
			tail->next = add_expr_to_list(e);
			tail = tail->next;
		}
	}
	return head;
}

Token parse_assign_operator() {
	Token t;
	t.token_type = -1;
	if (curr_token->token_type != OP_MULEQ &&
		 curr_token->token_type != OP_PLEQ  &&
		 curr_token->token_type != OP_MINEQ &&
		 curr_token->token_type != OP_DIVEQ &&
		 curr_token->token_type != OP_AEQ)
		return t;
	return *curr_token;
}

Token* parse_names() {
	Token name;
	Token* names = malloc(sizeof(Token) * 64);		//	silly!
	for (int i = 0; i < 64; ++i) {
		names[i].value.str_val = NULL;
	}
	int index = 0;
	name = parse_name();
	names[index++] = name;
	while(curr_token->token_type == OP_COMMA) {
		curr_token = get_next_token();
		name = parse_name();
		names[index++] = name;
	}
	return names;
}

PARAM_node* parse_param() {
	PARAM_node* p;
	Token param_name;
	TYPE_node* param_type;
	param_type = parse_type();
	if (param_type == NULL)
		lerr("bad param type", curr_token->line_no);
	curr_token = get_next_token();
	param_name = parse_name();

	p = make_param(param_name, param_type);
	return p;
}

STMT_node_list* parse_stmts() {
	expect(OP_BREACE_OPEN);
	STMT_node_list* head = NULL;
	STMT_node_list* tail = NULL;
	STMT_node* stmt = NULL;
	while(curr_token->token_type != OP_BREACE_CLOSE) {
		stmt = parse_stmt();
		if (stmt != NULL) {
			if (head == NULL) {
				head = add_stmt_to_list(stmt);
				tail = head;
			}
			else {
				tail->next = add_stmt_to_list(stmt);
				tail = tail->next;
			}
			continue;
		}
		lerr("wrong statement!", curr_token->line_no);
	}
	expect(OP_BREACE_CLOSE);
	return head;
}

STMT_node* parse_stmt() {
	STMT_node* s = NULL;
	TYPE_node* type;
	type = parse_type();
	if (type != NULL) {
		Token name;
		EXP_node* exp = NULL;
		curr_token = get_next_token();
		name = parse_name();
		if (curr_token->token_type == OP_AEQ) {
			curr_token = get_next_token();
			exp = parse_expression();
		}
		s = make_var_decl_stmt(type, name, exp);
	}
	else {
		switch(curr_token->token_type) {
			case KW_IF:
			{
				EXP_node* cond = NULL;
		  		STMT_node_list* body = NULL;
		  		BRANCH_node* branch = NULL;
		  		BRANCH_node_list* head = NULL;
		  		BRANCH_node_list* tail = NULL;
		  		curr_token = get_next_token();

		  		/* if branch */
		  		expect(OP_PAREN_OPEN);
		  		cond = parse_expression();
		  		expect(OP_PAREN_CLOSE);
		  		body = parse_stmts();
		  		branch = make_branch(cond, body);
		  		head = add_branch_to_list(branch);
		  		tail = head;

		  		/* else-if branches */
		  		while (curr_token->token_type == KW_ELSE && get_next_token_type() == KW_IF) {
		  			curr_token = get_next_token();
		  			curr_token = get_next_token();
		  			expect(OP_PAREN_OPEN);
			  		cond = parse_expression();
			  		expect(OP_PAREN_CLOSE);
			  		body = parse_stmts();
			  		branch = make_branch(cond, body);
			  		tail->next = add_branch_to_list(branch);
			  		tail = tail->next;
		  		}
		  		body = NULL;
		  		
		  		/* else body */
		  		if (curr_token->token_type == KW_ELSE) {
		  			curr_token = get_next_token();
		  			body = parse_stmts();
		  		}
		  		s = make_if_stmt(head, body);
		  		break;
			}
			case KW_FOR:
			{
				curr_token = get_next_token();
				if (curr_token->token_type == IDENT) {
					Token t = parse_name();
					EXP_node_list* exprs = NULL;
					STMT_node_list* body = NULL;
					expect(KW_IN);
					expect(OP_BRACKET_OPEN);
					exprs = parse_expressions();
					expect(OP_BRACKET_CLOSE);
					body = parse_stmts();
					s = make_for_in_stmt(t, exprs, body);
				}
				else {
					EXP_node* init = NULL;
					EXP_node* cond = NULL;
					EXP_node* inc = NULL;
					STMT_node_list* body = NULL;
					expect(OP_PAREN_OPEN);
					init = parse_expression();
					expect(OP_SEMICOLON);
					cond = parse_expression();
					expect(OP_SEMICOLON);
					inc = parse_expression();
					expect(OP_PAREN_CLOSE);
					body = parse_stmts();
					s = make_for_stmt(init, cond, inc, body);
				}
				break;
			}
			case KW_DO:
			{
				EXP_node* cond = NULL;
		  		STMT_node_list* body = NULL;
		  		curr_token = get_next_token();
		  		body = parse_stmts();
		  		expect(KW_WHILE);
		  		expect(OP_PAREN_OPEN);
		  		cond = parse_expression();
		  		expect(OP_PAREN_CLOSE);
		  		s = make_do_while_stmt(cond, body);
				break;
			}
			case KW_WHILE:
			{
				curr_token = get_next_token();
				EXP_node* cond = NULL;
		  		STMT_node_list* body = NULL;
		  		expect(OP_PAREN_OPEN);
		  		cond = parse_expression();
		  		expect(OP_PAREN_CLOSE);
		  		body = parse_stmts();
		  		s = make_while_stmt(cond, body);
		  		break;
				
			}
			case KW_RETURN:
			{
				curr_token = get_next_token();
				EXP_node* e = NULL;
				e = parse_expression();
				s = make_return_stmt(e);
				break;
			}
			case KW_BREAK:
			{
				s = make_break_stmt(*curr_token);
				curr_token = get_next_token();
				break;
			}
			case KW_CONTINUE:
			{
				s = make_continue_stmt(*curr_token);
				curr_token = get_next_token();
				break;
			}
			case KW_READ:
			{
				curr_token = get_next_token();
				Token* names = NULL;
				names = parse_names();
				s = make_input_stmt(names);
				break;
			}
			case KW_WRITE:
			{
				curr_token = get_next_token();
				EXP_node_list* expressions = NULL;
				expressions = parse_expressions();
				if (expressions == NULL)
					lerr("missing experession after 'write' statement", curr_token->line_no);
				s = make_output_stmt(expressions);
				break;
			}
			default:
 			{
 				EXP_node* exp = NULL;
 				exp = parse_expression();
 				s = make_expr_stmt(exp);
 				break;
 			}
        }
    }
    if (s != NULL)
	    expect(OP_SEMICOLON);
    return s;
}

PARAM_node_list* parse_params() {
	expect(OP_PAREN_OPEN);
	PARAM_node_list* head = NULL;
	PARAM_node_list* tail = NULL;
	PARAM_node* p = NULL;
	if (curr_token->token_type != OP_PAREN_CLOSE) {
		p = parse_param();
		head = add_param_to_list(p);
		tail = head;
		while (curr_token->token_type == OP_COMMA) {
			curr_token = get_next_token();
			p = parse_param();
			tail->next = add_param_to_list(p);
			tail = tail->next;
		}
	}
	expect(OP_PAREN_CLOSE);
	return head;	
}

FNDECL_node* parse_function() {
	FNDECL_node* n;
	Token func_name;
	PARAM_node_list* params; 
	TYPE_node* ret_type;
	STMT_node_list* body;

	ret_type = parse_type();
	curr_token = get_next_token();
	func_name = parse_name();
	params = parse_params();
	body = parse_stmts();

	n = make_func(func_name, params, ret_type, body);
	return n;
}

FUNC_node_list* parse_functions() {
	FUNC_node_list* head = NULL;
	FUNC_node_list* tail = NULL;
	FNDECL_node* f = NULL;
	while (curr_token->token_type != _EOF) {
		f = parse_function();
		if (head == NULL) {
			head = add_func_to_list(f);
			tail = head;
		}
		else {
			tail->next = add_func_to_list(f);
			tail = tail->next;
		}
	}
	return head;
}

PROGRAM_node* parse_program(Token* tokens, const char* filename) {
	PROGRAM_node* p = make_program_node("");
	curr_token = tokens;
	p->functions = parse_functions();
	return p;
}
#include "../../../include/frontend/symbol/type_checker.h"
#include "../../../include/frontend/symbol/name_resolver.h"

TYPE_node* get_var_type(Token key, Symbol_table* t) {
	Symbol* s = get_sym(t, key);
	if (s != NULL) {
		if (s->kind != func_sym)
			return s->val.var_val;
		else
			_report_err("wrong construct for function call", NULL, NULL, NULL, key);
	}
	return NULL;
}

Symbol* get_sym(Symbol_table* t, Token key) {
	Symbol* s;
	if ((s = get_symbol(t, key)) == NULL) {
		if ((s = get_symbol(global_scope, key)) == NULL) {
			char s[100];
			sprintf(s, "'%s' is not defined", key.value.str_val);
			_report_err(s, NULL, NULL, NULL, key);
		}
	}
	return s;
}

PARAM_node_list* get_function_params(Token key, Symbol_table* t) {
	Symbol* s = get_sym(t, key);
	if (s != NULL && s->kind == func_sym) {
		return s->val.func_sym_val.params;
	} 
	char str[100];
	sprintf(str, "called object '%s' is not a function", key.value.str_val);
	_report_err(str, NULL, NULL, NULL, key);
	return NULL;
}

TYPE_node* get_function_type(Token key, Symbol_table* t) {
	Symbol* s = get_sym(t, key);
	if (s != NULL && s->kind == func_sym) {
		return s->val.func_sym_val.return_type;
	}
	char str[100];
	sprintf(str, "called object '%s' is not a function", key.value.str_val);
	_report_err(str, NULL, NULL, NULL, key);
	return NULL;
}

TYPE_node* get_binary_type(EXP_node* e, Symbol_table* t) {
	TYPE_node* type = NULL;
	if (e->val.binaryExp.binary_exp_kind == logical_exp) {
		type = BOOL;
		TYPE_node* left = get_expr_type(e->val.binaryExp.left, t);
		TYPE_node* right = get_expr_type(e->val.binaryExp.right, t);
		if (left != NULL && right != NULL) {
			if (check_types(left, type) != TYPE_MATCH || check_types(right, type) != TYPE_MATCH)
				_report_err("invalid operands for operator ", str_operator(e), left, right, e->val.binaryExp.operator);
		}
		else {
			type = NULL;
		}
	}
	else if (e->val.binaryExp.binary_exp_kind == equality_exp) {
		type = BOOL;
		TYPE_node* left = get_expr_type(e->val.binaryExp.left, t);
		TYPE_node* right = get_expr_type(e->val.binaryExp.right, t);
		if (left != NULL && right != NULL) {
			if (check_types(left, right) != TYPE_MATCH) 
				_report_err("invalid operands for operator ", str_operator(e), left, right, e->val.binaryExp.operator);
		}
		else {
			type = NULL;
		}
	}
	else if (e->val.binaryExp.binary_exp_kind == relational_exp ||
				  e->val.binaryExp.binary_exp_kind == additive_exp ||
				  e->val.binaryExp.binary_exp_kind == multiplicative_exp) 
	{
		type = INT;
		TYPE_node* left = get_expr_type(e->val.binaryExp.left, t);
		TYPE_node* right = get_expr_type(e->val.binaryExp.right, t);
		if (left != NULL && right != NULL) {
			if (check_types(left, type) != TYPE_MATCH || check_types(right, type) != TYPE_MATCH) {
				if	(check_types(left, FLOAT) != TYPE_MATCH || check_types(right, FLOAT) != TYPE_MATCH)
					_report_err("invalid operands for operator ", str_operator(e), left, right, e->val.binaryExp.operator);
				else type =  FLOAT;
			}
			if (e->val.binaryExp.binary_exp_kind == relational_exp)
				type = BOOL;
		}
		else {
			type = NULL;
		}
	}
	else {
		type = BOOL;
		TYPE_node* target_type = get_expr_type(e->val.binaryExp.left, t);
		TYPE_node* assign_type = get_expr_type(e->val.binaryExp.right, t);
		if (target_type != NULL && assign_type != NULL) {
			if (check_types(target_type, assign_type) != TYPE_MATCH) {
				_report_err("invalid operands for operator ", str_operator(e), target_type, assign_type, e->val.binaryExp.operator);
			}
		}
		else {
			type = NULL;
		}
	}
	return type;
}

TYPE_node* get_unary_type(EXP_node* e, Symbol_table* t) {
	TYPE_node* type = get_expr_type(e->val.unaryExp.operand, t);
	if (type != NULL) {
		switch (e->val.unaryExp.unary_exp_kind) {
			case negation_exp:
			{
				if (check_types(type, BOOL) != TYPE_MATCH)
					_report_err("invalid operand for operator ", str_operator(e), type, NULL, e->val.unaryExp.operator);
				break;
			}
			case inverse_exp:
			{
				if (check_types(type, INT) != TYPE_MATCH && check_types(type, FLOAT) != TYPE_MATCH) {
					_report_err("invalid operand for operator ", str_operator(e), type, NULL, e->val.unaryExp.operator);
				}
				break;
			}
			case reference_exp:
			{
				// NOT IMPLEMENTED
				break;
			}
			case dereference_exp:
			{
				// NOT IMPLEMENTED
				break;
			}
			default:
			{
				_report_err("unknown unary kind", NULL, NULL, NULL, e->val.unaryExp.operator);
			}
		}
	}
	return type;
}

TYPE_node* get_expr_type(EXP_node* exp, Symbol_table* t) {
	TYPE_node* type = NULL;
	if (exp->kind == binary_exp)
		type = get_binary_type(exp, t);
	else if (exp->kind == unary_exp)
		type = get_unary_type(exp, t);
	else {
		switch(exp->val.primaryExp.primary_exp_kind) {
				case variable_exp:
				{
					type = get_var_type(exp->val.primaryExp.primary_exp_val.constantExp, t);
					break;
				}
				case string_exp:
				{
					type = STRING;
					break;
				}
				case integer_exp:
				{
					type = INT;
					break;
				}
				case float_exp:
				{
					type = FLOAT;
					break;
				}
				case boolean_exp:
				{
					type = BOOL;
					break;
				}
				case call_exp:
				{
					type = type_check_invoke(exp->val.primaryExp.primary_exp_val.callExp.name,
												 	 exp->val.primaryExp.primary_exp_val.callExp.arguments, t);
					break;
				}
				default:
				{
					// ???
					break;
				}
		}
	}
	return type;
}

int check_types(TYPE_node* a, TYPE_node* b) {
	if (a->kind != b->kind)
		return TYPE_KIND_MISMATCH;
	else if (a->kind == primary_type) {
		if (a->val.primary_type.primary_type_kind == VoidType || 
				b->val.primary_type.primary_type_kind == VoidType)
			return VOID_TYPE_USE;
		else if (a->val.primary_type.primary_type_kind != b->val.primary_type.primary_type_kind)
			return TYPE_MISMATCH;
	}
	else
		return check_types(a->val.pointer_type_value, b->val.pointer_type_value);
	return TYPE_MATCH;
}

TYPE_node* type_check_invoke(Token key, EXP_node_list* args, Symbol_table* t) {
	PARAM_node_list* params;
	TYPE_node* ret_type = NULL;
	EXP_node_list* head = args;
	int params_num;
	int args_num;
	params = get_function_params(key, t);
	if (params != NULL) {
		ret_type = get_function_type(key, t);
		params_num = get_params_count(params);
		args_num = get_exprs_count(args);
		if (params_num != args_num) {
			int n = MIN(args_num, params_num);
			for (int i = 0; i < n; ++i) {
				TYPE_node* arg = get_expr_type(head->elem, t);
				if (check_types(arg, params->elem->type) != TYPE_MATCH) {
					char s[100];
					sprintf(s, "incompatible type for argument %d of ‘%s’: expected '%s' but argument is of type '%s'", i, key.value.str_val, str_type(params->elem->type), str_type(arg));
					_report_err(s, NULL, NULL, NULL, key);
				}
				head = head->next;
				params = params->next;
			}
			char s[100];
			sprintf(s, "number of parameters do not match number of arguments: required: %d, got: %d", params_num, args_num);
			_report_err(s, NULL, NULL, NULL, key);
		}
		else {
			for (int i = 0; i < args_num; ++i) {
				TYPE_node* arg = get_expr_type(head->elem, t);
				if (check_types(arg, params->elem->type) != TYPE_MATCH) {
					char s[100];
					sprintf(s, "incompatible type for argument %d of ‘%s’: expected '%s' but argument is of type '%s'", i, key.value.str_val, str_type(params->elem->type), str_type(arg));
					_report_err(s, NULL, NULL, NULL, key);
				}
				head = head->next;
				params = params->next;
			}
		}
	}
	else {
		args_num = get_exprs_count(args);
		for (int i = 0; i < args_num; ++i) {
			get_expr_type(head->elem, t);
			head = head->next;
		}
	}
	return ret_type;
}

void type_check_var_decl(Symbol_table* t, TYPE_node* target_type, EXP_node* exp) {
	if (exp != NULL) {
		TYPE_node* expr_type = get_expr_type(exp, t);
		if (expr_type != NULL) {
			if (check_types(target_type, expr_type) != TYPE_MATCH) {
				Token token = get_expr_token(exp);
				_report_err("declaration type mismatch", NULL, target_type, expr_type, token);	// fix !!!
			}
		}
	}
}

void type_check_return(Symbol_table* t, EXP_node* exp, Token name) {
	TYPE_node* return_type = NULL;
	TYPE_node* func_ret_type = get_function_type(name, global_scope);
	if (exp != NULL) {
		return_type = get_expr_type(exp, t);
		if (check_types(func_ret_type, BOOL) == VOID_TYPE_USE) {
			_report_err("returning non-void from void function", NULL, NULL, NULL, name);
		}
		if (check_types(func_ret_type, return_type) != VOID_TYPE_USE) {
			if (check_types(func_ret_type, return_type) != TYPE_MATCH) {
				_report_err("return type mismatched function type", NULL, func_ret_type, return_type, name);
			}
		}
	}
	else {
		if (check_types(func_ret_type, BOOL) != VOID_TYPE_USE) {
			_report_err("returning void from non-void function", NULL, NULL, NULL, name);
		}
	}
}

void type_check_input(Token key, Symbol_table* t) {
	TYPE_node* type = get_var_type(key, t);
	if (check_types(type, BOOL) == TYPE_MATCH)
		_report_err("variable type cannot be boolean here", NULL, NULL, NULL, key);
}

void init_pointer_types() {
	BOOL_PTR = make_pointer_type(BOOL);
	INT_PTR = make_pointer_type(INT);
	VOID_PTR = make_pointer_type(VOID);
	FLOAT_PTR = make_pointer_type(FLOAT);
	STRING_PTR = make_pointer_type(STRING);
}

void init_primitive_types() {
	Token t;
	BOOL = make_primary_type(BooleanType, t);
	INT = make_primary_type(IntType, t);
	VOID = make_primary_type(VoidType, t);
	FLOAT = make_primary_type(FloatType, t);
	STRING = make_primary_type(StringType, t);
}
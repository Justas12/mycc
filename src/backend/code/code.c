#include <stdlib.h>
#include <string.h>
#include "../../../include/frontend/symbol/type_checker.h"
#include "../../../include/backend/code/code.h"
#include "../../../include/utils/utils.h"

Code* add_code(int name) {
	Code* c = malloc(sizeof(Code));
	Instruction i = get_instr_by_name(name);
	c->instr = i;
	c->offset = offset;
	c->val.label_val = -1;
	c->jump_slot = NULL;
	if (chead == NULL) {
		chead = c;
		ctail = chead;
	}
	else {
		ctail->next = c;
		ctail = ctail->next;
	}
	offset = offset + i.num_ops + 1;
	return c;
}

void code_primary_exp(EXP_node* exp, Symbol_table* t) {
	switch(exp->val.primaryExp.primary_exp_kind) {
		case variable_exp:
		{
			Symbol* s = get_symbol(t, exp->val.primaryExp.primary_exp_val.constantExp);
			if (s != NULL) {
				Code* c = add_code(I_GET_L);
				c->val.label_val = s->stack_slot;
			}
			break;
		}
		case string_exp:
		{

									//	FIX ???
			/* --------------------------------------------------- */
			int len = strlen(exp->val.primaryExp.primary_exp_val.constantExp.value.str_val);
			int i = get_instr_index_by_name(I_STRING_PUSH);
			Instruction ins;
			ins.op_code = 0x34;
			ins.num_ops = len + 1;
			ins.name = I_STRING_PUSH;
			instructions[i] = ins;
			/* --------------------------------------------------- */

			Code* c = add_code(I_STRING_PUSH);
			c->val.op_val = exp->val.primaryExp.primary_exp_val.constantExp;
			break;
		}
		case integer_exp:
		{
			Code* c = add_code(I_INT_PUSH);
			c->val.op_val = exp->val.primaryExp.primary_exp_val.constantExp;
			break;
		}
		case float_exp:
		{
			Code* c = add_code(I_FLOAT_PUSH);
			c->val.op_val = exp->val.primaryExp.primary_exp_val.constantExp;
			break;
		}
		case boolean_exp:
		{
			Code* c = add_code(I_INT_PUSH);
			c->val.op_val = exp->val.primaryExp.primary_exp_val.constantExp;
			break;
		}
		case call_exp:
		{
			int index = 0;
			EXP_node_list* args = exp->val.primaryExp.primary_exp_val.callExp.arguments;
			while (args != NULL) {
				code_expr(args->elem, t);
				index += 1;
				args = args->next;
			}
			Code* c = add_code(I_CALL);
			c->val.call_val.num_args = index;
			c->val.call_val.call_name = exp->val.primaryExp.primary_exp_val.callExp.name;
			break;
		}
		default:
		{
			lerr("Wrong primary-expr type!", 1);
		}
	}
}

void code_binary_exp(EXP_node* e, Symbol_table* t) {
	switch(e->val.binaryExp.binary_exp_kind) {
		case logical_exp:
		{
			code_expr(e->val.binaryExp.left, t);
			code_expr(e->val.binaryExp.right, t);
			if (e->val.binaryExp.operator.token_type == OP_LAND)
				add_code(I_AND);
			else
				add_code(I_OR);
			break;
		}
		case equality_exp:
		{
			code_expr(e->val.binaryExp.left, t);
			code_expr(e->val.binaryExp.right, t);
			TYPE_node* type = get_expr_type(e, t);
			if (type != NULL) {
				TYPE_node* type = get_expr_type(e->val.binaryExp.left, t);
				switch(type->val.primary_type.primary_type_kind) {
					case IntType:
					{
						if (e->val.binaryExp.operator.token_type == OP_CEQ)
							add_code(I_INT_EQ);
						else if (e->val.binaryExp.operator.token_type == OP_NEQ)
							add_code(I_INT_NEQ);
						else
							lerr("Wrong equality operator!", 1);
						break;
					}
					case BooleanType:
					{
						if (e->val.binaryExp.operator.token_type == OP_CEQ)
							add_code(I_INT_EQ);
						else if (e->val.binaryExp.operator.token_type == OP_NEQ)
							add_code(I_INT_NEQ);
						else
							lerr("Wrong equality operator!", 1);
						break;
					}
					case StringType:
					{
						if (e->val.binaryExp.operator.token_type == OP_CEQ)
							add_code(I_STRING_EQ);
						else if (e->val.binaryExp.operator.token_type == OP_NEQ)
							add_code(I_STRING_NEQ);
						else
							lerr("Wrong equality operator!", 1);
						break;
					}
					case FloatType:
					{
						if (e->val.binaryExp.operator.token_type == OP_CEQ)
							add_code(I_FLOAT_EQ);
						else if (e->val.binaryExp.operator.token_type == OP_NEQ)
							add_code(I_FLOAT_NEQ);
						else
							lerr("Wrong equality operator!", 1);
						break;
					}
					default:
					{
						lerr("Wrong type!", 1);
					}
				}
			}
			break;
		}
		case relational_exp:
		{
			code_expr(e->val.binaryExp.left, t);
			code_expr(e->val.binaryExp.right, t);
			TYPE_node* type = get_expr_type(e, t);
			if (type != NULL) {
				TYPE_node* type = get_expr_type(e->val.binaryExp.left, t);
				if (type->val.primary_type.primary_type_kind == IntType) {
					switch(e->val.binaryExp.operator.token_type) {
						case OP_LS:
						{
							add_code(I_INT_LESSER);
							break;
						}
						case OP_GR:
						{
							add_code(I_INT_GREATER);
							break;
						}
						case OP_LSE:
						{
							add_code(I_INT_LESSEREQ);
							break;
						}
						case OP_GRE:
						{
							add_code(I_INT_GRESTEREQ);
							break;
						}
						default:
						{
							lerr("Wrong realational operator!", 1);
							break;
						}
					}
				}
				else {
					switch(e->val.binaryExp.operator.token_type) {
						case OP_LS:
						{
							add_code(I_FLOAT_LESSER);
							break;
						}
						case OP_GR:
						{
							add_code(I_FLOAT_GREATER);
							break;
						}
						case OP_LSE:
						{
							add_code(I_FLOAT_LESSEREQ);
							break;
						}
						case OP_GRE:
						{
							add_code(I_FLOAT_GREATEREQ);
							break;
						}
						default:
						{
							lerr("Wrong realational operator!", 1);
							break;
						}
					}
				}
			}
			break;
		}
		case additive_exp:
		{
			code_expr(e->val.binaryExp.left, t);
			code_expr(e->val.binaryExp.right, t);
			TYPE_node* type = get_expr_type(e, t);
			if (type != NULL) {
				if (type->val.primary_type.primary_type_kind == IntType) {
					if (e->val.binaryExp.operator.token_type == OP_PLUS)
						add_code(I_INT_ADD);
					else if (e->val.binaryExp.operator.token_type == OP_MINUS)
						add_code(I_INT_SUB);
					else
						lerr("Wrong additive operator!", 1);
				}
				else {
					if (e->val.binaryExp.operator.token_type == OP_PLUS)
						add_code(I_FLOAT_ADD);
					else if (e->val.binaryExp.operator.token_type == OP_MINUS)
						add_code(I_FLOAT_SUB);
					else
						lerr("Wrong additive operator!", 1);
				}
			}
			break;
		}
		case multiplicative_exp:
		{
			code_expr(e->val.binaryExp.left, t);
			code_expr(e->val.binaryExp.right, t);
			TYPE_node* type = get_expr_type(e, t);
			if (type != NULL) {
				if (type->val.primary_type.primary_type_kind == IntType) {
					if (e->val.binaryExp.operator.token_type == OP_MULT)
						add_code(I_INT_MULT);
					else if (e->val.binaryExp.operator.token_type == OP_DIV)
						add_code(I_INT_DIV);
					else if (e->val.binaryExp.operator.token_type == OP_PERC)
						add_code(I_INT_MOD);
					else
						lerr("Wrong additive operator!", 1);
				}
				else {
					if (e->val.binaryExp.operator.token_type == OP_MULT)
						add_code(I_FLOAT_MULT);
					else if (e->val.binaryExp.operator.token_type == OP_DIV)
						add_code(I_FLOAT_DIV);
					else if (e->val.binaryExp.operator.token_type == OP_PERC)
						add_code(I_FLOAT_MOD);
					else
						lerr("Wrong additive operator!", 1);
				}
			}
			break;
		}
		case assign_exp:
		{
			EXP_node* target = e->val.binaryExp.left;
			Symbol* s = get_symbol(t, target->val.primaryExp.primary_exp_val.constantExp);
			if (s != NULL) {
				if (e->val.binaryExp.operator.token_type != OP_AEQ) {
					Code* c = add_code(I_GET_L);
					code_expr(e->val.binaryExp.right, t);
					c->val.label_val = s->stack_slot;
					switch(e->val.binaryExp.operator.token_type) {
						case OP_MULEQ:
						{
							if (get_type_kind(s->val.var_val) == IntType)
								add_code(I_INT_MULT);
							else
								add_code(I_FLOAT_MULT);
							break;
						}
						case OP_PLEQ:
						{
							if (get_type_kind(s->val.var_val) == IntType)
								add_code(I_INT_ADD);
							else
								add_code(I_FLOAT_ADD);
							break;
						}
						case OP_MINEQ:
						{
							if (get_type_kind(s->val.var_val) == IntType)
								add_code(I_INT_SUB);
							else
								add_code(I_INT_SUB);
							break;
						}
						case OP_DIVEQ:
						{
							if (get_type_kind(s->val.var_val) == IntType)
								add_code(I_INT_DIV);
							else
								add_code(I_FLOAT_DIV);
							break;
						}
						default:
						{
							printf("bad assign operator!\n");
						}
					}
				}
				else
					code_expr(e->val.binaryExp.right, t);
				Code* c = add_code(I_SET_L);
				c->val.label_val = s->stack_slot;
			}
			break;
		}
		default:
		{
			lerr("Wrong binary-expr type!", 1);
		}
	}
}

void code_unary_exp(EXP_node* e, Symbol_table* tb) {
	switch(e->val.unaryExp.unary_exp_kind) {
		case inverse_exp:
		{
			TYPE_node* type = get_expr_type(e->val.unaryExp.operand, tb);
			if (type != NULL) {
				Token t;
				Code* c;
				if (get_type_kind(type) == IntType) {
					t = make_int_token(0, 0);
					c = add_code(I_INT_PUSH);
					c->val.op_val = t;
					code_expr(e->val.unaryExp.operand, tb);
					add_code(I_INT_SUB);
				}
				else {
					t = make_float_token(0.0, 0);
					c = add_code(I_FLOAT_PUSH);
					c->val.op_val = t;
					code_expr(e->val.unaryExp.operand, tb);
					add_code(I_FLOAT_SUB);
				}
			}
			break;
		}
		case negation_exp:
		{
			get_expr_type(e->val.unaryExp.operand, tb);
			code_expr(e->val.unaryExp.operand, tb);
			add_code(I_NOT);
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
			lerr("Wrong unary-expr type!", 1);
		}
	}
}

void code_expr(EXP_node* e, Symbol_table* t) {
	switch(e->kind) {
		case primary_exp:
		{
			code_primary_exp(e, t);
			break;
		}
		case binary_exp:
		{
			code_binary_exp(e, t);
			break;
		}
		case unary_exp:
		{
			code_unary_exp(e, t);
			break;
		}
		default:
		{
			lerr("Wrong expression type!", 1);
		}
	}
}

void print_opcodes() {
	Code* head = chead;
	printf("[");
	if (head != NULL) {
		printf("%x", head->instr.op_code);
		head = head->next;
	}
	while(head != NULL) {
		printf(", %x", head->instr.op_code);
		head = head->next;
	}
	printf("]\n");
}

void print_code() {
	printf("%-7s%-20s%-50s\n", "OFFSET", "|INSTRUCTION", "|OPERANDS");
	printf("-------+-------------------+---------------\n");
	Code* head = chead;
 
	while(head != NULL) {
		printf("%04d   ", head->offset);
		printf("|");
		printf("%-19s", str_instructions[head->instr.name]);
		printf("|");
		if (head->instr.name == I_CALL)
			printf("%d, %d\n", head->val.call_val.label, head->val.call_val.num_args);
		else if (head->instr.name == I_INT_PUSH)
			printf("%d\n", head->val.op_val.value.int_val);
		else if (head->instr.name == I_FLOAT_PUSH)
			printf("%f\n", head->val.op_val.value.float_val);
		else if (head->instr.name == I_STRING_PUSH)
			printf("%s\n", head->val.op_val.value.str_val);
		else if (head->val.label_val != -1)
			printf("%d\n", head->val.label_val);
		else
			printf("\n");
		head = head->next;
	}
}

int* get_code() {
	int* arr = malloc(sizeof(int)*offset);
	memset((void*)arr, -1, offset);
	int index = 0;
	Code* head = chead;
	while(head != NULL) {
		switch(head->instr.name) {
			case I_INT_ADD:
			{
				arr[index++] = I_INT_ADD;
				break;
			}
			case I_INT_SUB:
			{
				arr[index++] = I_INT_SUB;
				break;
			}
			case I_INT_MULT:
			{
				arr[index++] = I_INT_MULT;
				break;
			}
			case I_INT_DIV:
			{
				arr[index++] = I_INT_DIV;
				break;
			}
			case I_INT_MOD:
			{
				arr[index++] = I_INT_MOD;
				break;
			}
			case I_FLOAT_ADD:
			{
				arr[index++] = I_FLOAT_ADD;
				break;
			}
			case I_FLOAT_SUB:
			{
				arr[index++] = I_FLOAT_SUB;
				break;
			}
			case I_FLOAT_MULT:
			{
				arr[index++] = I_FLOAT_MULT;
				break;
			}
			case I_FLOAT_DIV:
			{
				arr[index++] = I_FLOAT_DIV;
				break;
			}
			case I_FLOAT_MOD:
			{
				arr[index++] = I_FLOAT_MOD;
				break;
			}
			case I_INT_LESSER:
			{
				arr[index++] = I_INT_LESSER;
				break;
			}
			case I_INT_LESSEREQ:
			{
				arr[index++] = I_INT_LESSEREQ;
				break;
			}
			case I_INT_GREATER:
			{
				arr[index++] = I_INT_GREATER;
				break;
			}
			case I_INT_GRESTEREQ:
			{
				arr[index++] = I_INT_GRESTEREQ;
				break;
			}
			case I_FLOAT_LESSER:
			{
				arr[index++] = I_FLOAT_LESSER;
				break;
			}
			case I_FLOAT_LESSEREQ:
			{
				arr[index++] = I_FLOAT_LESSEREQ;
				break;
			}
			case I_FLOAT_GREATER:
			{
				arr[index++] = I_FLOAT_GREATER;
				break;
			}
			case I_FLOAT_GREATEREQ:
			{
				arr[index++] = I_FLOAT_GREATEREQ;
				break;
			}
			case I_POP:
			{
				arr[index++] = I_POP;
				break;
			}
			case I_INT_PUSH:
			{
				arr[index++] = I_INT_PUSH;
				if (head->val.op_val.token_type == LIT_INT)
					arr[index++] = head->val.op_val.value.int_val;
				else if (head->val.op_val.token_type == KW_TRUE)
					arr[index++] = 1;
				else
					arr[index++] = 0;
				break;
			}
			case I_FLOAT_PUSH:
			{
				arr[index++] = I_FLOAT_PUSH;
				Float f;
				f.val = head->val.op_val.value.float_val;
				for (int i = 0; i < 4; ++i)
					arr[index++] = f.bytes[i];
				break;
			}
			case I_STRING_PUSH:
			{
				arr[index++] = I_STRING_PUSH;
				char* str = head->val.op_val.value.str_val;
				while(*str != '\0') {
					arr[index++] = *str;
					str++;
				}
				arr[index++] = '\0';
				break;
			}
			case I_GET_L:
			{
				arr[index++] = I_GET_L;
				arr[index++] = head->val.label_val;
				break;
			}
			case I_SET_L:
			{
				arr[index++] = I_SET_L;
				arr[index++] = head->val.label_val;
				break;
			}
			case I_BR:
			{
				arr[index++] = I_BR;
				arr[index++] = head->val.label_val;
				break;
			}
			case I_BZ:
			{
				arr[index++] = I_BZ;
				arr[index++] = head->val.label_val;
				break;
			}
			case I_RET:
			{
				arr[index++] = I_RET;
				break;
			}
			case I_RET_V:
			{
				arr[index++] = I_RET_V;
				break;
			}
			case I_CALL_BEGIN:
			{
				// ???
				break;
			}
			case I_CALL:
			{
				arr[index++] = I_CALL;
				arr[index++] = head->val.call_val.label;
				arr[index++] = head->val.call_val.num_args;
				break;
			}
			case I_EXIT:
			{
				arr[index++] = I_EXIT;
				break;
			}
			case I_AND:
			{
				arr[index++] = I_AND;
				break;
			}
			case I_OR:
			{
				arr[index++] = I_OR;
				break;
			}
			case I_INT_EQ:
			{
				arr[index++] = I_INT_EQ;
				break;
			}
			case I_INT_NEQ:
			{
				arr[index++] = I_INT_NEQ;
				break;
			}
			case I_FLOAT_EQ:
			{
				arr[index++] = I_FLOAT_EQ;
				break;
			}
			case I_FLOAT_NEQ:
			{
				arr[index++] = I_FLOAT_NEQ;
				break;
			}
			case I_STRING_EQ:
			{
				arr[index++] = I_STRING_EQ;
				break;
			}
			case I_STRING_NEQ:
			{
				arr[index++] = I_STRING_NEQ;
				break;
			}
			case I_INT_PRINT:
			{
				arr[index++] = I_INT_PRINT;
				break;
			}
			case I_FLOAT_PRINT:
			{
				arr[index++] = I_FLOAT_PRINT;
				break;
			}
			case I_STRING_PRINT:
			{
				arr[index++] = I_STRING_PRINT;
				break;
			}
			case I_INT_READ:
			{
				arr[index++] = I_INT_READ;
				arr[index++] = head->val.label_val;
				break;
			}
			case I_FLOAT_READ:
			{
				arr[index++] = I_FLOAT_READ;
				arr[index++] = head->val.label_val;
				break;
			}
			case I_STRING_READ:
			{
				arr[index++] = I_STRING_READ;
				arr[index++] = head->val.label_val;
				break;
			}
			case I_NOT:
			{
				arr[index++] = I_NOT;
				break;
			}
			default:
			{
				printf("Wrong instr name!\n");
			}
		}
		head = head->next;
	}
	return arr;
}
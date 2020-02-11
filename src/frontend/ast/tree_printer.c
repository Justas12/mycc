#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../include/frontend/ast/tree_printer.h"
#include "../../../include/utils/utils.h"

void print_assign_op(Token t) {
	switch(t.token_type) {
		case OP_MULEQ:
		{
			printf("*=\n");
			break;
		}
		case OP_PLEQ:
		{
			printf("+=\n");
			break;
		}
		case OP_MINEQ:
		{
			printf("-=\n");
			break;
		}
		case OP_DIVEQ:
		{
			printf("/=\n");
			break;
		}
		case OP_AEQ:
		{
			printf("=\n");
			break;
		}
		default:
		{
			printf("bad assign operator!\n");
		}
	}
}

void print_type(TYPE_node* type, int ident_level) {
	int level = ident_level;
	printf("Type: \n");
	printf("%*s", level+5, "");
	printf("kind: ");
	switch(type->kind) {
		case primary_type:
		{
			printf("PrimaryType \n");
			printf("%*s", level+5, "");
			printf("value: ");
			switch(type->val.primary_type.primary_type_kind) {
				case IntType:
				{
					printf("IntType\n");
					break;
				}
				case BooleanType:
				{
					printf("BooleanType\n");
					break;
				}
				case StringType:
				{
					printf("StringType\n");
					break;
				}
				case FloatType:
				{
					printf("FloatType\n");
					break;
				}
				case VoidType:
				{
					printf("VoidType\n");
					break;
				}
				default:
				{
					lerr("Unknown primary type!", type->val.primary_type.primary_type_value.line_no);
				}
			}
			break;
		}
		case pointer_type:
		{
			printf("PointerType \n");
			printf("%*s", level+5, "");
			printf("value: ");
			print_type(type->val.pointer_type_value, level+5);
			break;
		}
		default:
		{
			lerr("Unknown type kind!", 1);
		}
	}
}

void print_expr(EXP_node* e, int ident_level) {
	if (e != NULL) {
		switch(e->kind) {
			case primary_exp:
			{
				printf("PrimaryExp:\n");
				print_primary_expr(e, ident_level);
				break;
			}
			case binary_exp:
			{
				printf("BinaryExp:\n");
				print_binary_expr(e, ident_level);
				break;
			}
			case unary_exp:
			{
				printf("UnaryExp:\n");
				print_unary_expr(e, ident_level);
				break;
			}
			default:
			{
				lerr("Wrong expression type!", 1);
			}
		}
	}
	else
		printf("\n");
}

void print_unary_expr(EXP_node* e, int ident_level) {
	int level = ident_level;
	switch(e->val.unaryExp.unary_exp_kind) {
		case negation_exp:
		{
			printf("%*s", level, "");
			printf("operator: !\n");
			printf("%*s", level, "");
			printf("operand: ");
			print_expr(e->val.unaryExp.operand, level+5);
			break;
		}
		case inverse_exp:
		{
			printf("%*s", level, "");
			printf("operator: -\n");
			printf("%*s", level, "");
			printf("operand: ");
			print_expr(e->val.unaryExp.operand, level+5);
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

void print_binary_expr(EXP_node* e, int ident_level) {
	int level = ident_level;
	Token op = e->val.binaryExp.operator;
	switch(e->val.binaryExp.binary_exp_kind) {
		case logical_exp:
		{
			printf("%*s", level, "");
			printf("operator: ");
			if (op.token_type == OP_LOR)
				printf("||\n");
			else
				printf("&&\n");
			printf("%*s", level, "");
			printf("left: ");
			print_expr(e->val.binaryExp.left, level+5);
			printf("%*s", level, "");
			printf("right: ");
			print_expr(e->val.binaryExp.right, level+5);
			break;
		}
		case equality_exp:
		{
			printf("%*s", level, "");
			printf("operator: ");
			if (op.token_type == OP_CEQ)
				printf("==\n");
			else
				printf("!=\n");
			printf("%*s", level, "");
			printf("left: ");
			print_expr(e->val.binaryExp.left, level+5);
			printf("%*s", level, "");
			printf("right: ");
			print_expr(e->val.binaryExp.right, level+5);
			break;
		}
		case relational_exp:
		{
			printf("%*s", level, "");
			printf("operator: ");
			if (op.token_type == OP_LS)
				printf("<\n");
			else if (op.token_type == OP_GR)
				printf(">\n");
			else if (op.token_type == OP_GRE)
				printf(">=\n");
			else
				printf("<=\n");
			printf("%*s", level, "");
			printf("left: ");
			print_expr(e->val.binaryExp.left, level+5);
			printf("%*s", level, "");
			printf("right: ");
			print_expr(e->val.binaryExp.right, level+5);
			break;
		}
		case additive_exp:
		{
			printf("%*s", level, "");
			printf("operator: ");
			if (op.token_type == OP_PLUS)
				printf("+\n");
			else
				printf("-\n");
			printf("%*s", level, "");
			printf("left: ");
			print_expr(e->val.binaryExp.left, level+5);
			printf("%*s", level, "");
			printf("right: ");
			print_expr(e->val.binaryExp.right, level+5);
			break;
		}
		case multiplicative_exp:
		{
			printf("%*s", level, "");
			printf("operator: ");
			if (op.token_type == OP_MULT)
				printf("*\n");
			else if (op.token_type == OP_DIV)
				printf("/\n");
			else
				printf(":\n");
			printf("%*s", level, "");
			printf("left: ");
			print_expr(e->val.binaryExp.left, level+5);
			printf("%*s", level, "");
			printf("right: ");
			print_expr(e->val.binaryExp.right, level+5);
			break;
		}
		case assign_exp:
		{
			printf("%*s", level, "");
			printf("operator: ");
			print_assign_op(e->val.binaryExp.operator);
			printf("%*s", level, "");
			printf("left: ");
			print_expr(e->val.binaryExp.left, level+5);
			printf("%*s", level, "");
			printf("right: ");
			print_expr(e->val.binaryExp.right, level+5);
			break;
		}
		default:
		{
			lerr("Wrong binary-expr type!", 1);
		}
	}
}

void print_primary_expr(EXP_node* exp, int ident_level) {
	int level = ident_level;
	switch(exp->val.primaryExp.primary_exp_kind) {
		case variable_exp:
		{
			printf("%*s", level, "");
			printf("kind: ");
			level = level + 5;
			printf("Variable:\n");
			printf("%*s", level, "");
			printf("value: %s\n", exp->val.primaryExp.primary_exp_val.constantExp.value.str_val);
			break;
		}
		case string_exp:
		{
			printf("%*s", level, "");
			printf("kind: ");
			level = level + 5;
			printf("String:\n");
			printf("%*s", level, "");
			printf("value: %s\n", exp->val.primaryExp.primary_exp_val.constantExp.value.str_val);
			break;
		}
		case integer_exp:
		{
			printf("%*s", level, "");
			printf("kind: ");
			level = level + 5;
			printf("Integer:\n");
			printf("%*s", level, "");
			printf("value: %d\n", exp->val.primaryExp.primary_exp_val.constantExp.value.int_val);
			break;
		}
		case float_exp:
		{
			printf("%*s", level, "");
			printf("kind: ");
			level = level + 5;
			printf("Float:\n");
			printf("%*s", level, "");
			printf("value: %f\n", exp->val.primaryExp.primary_exp_val.constantExp.value.float_val);
			break;
		}
		case boolean_exp:
		{
			printf("%*s", level, "");
			printf("kind: ");
			level = level + 5;
			printf("Boolean:\n");
			printf("%*s", level, "");
			if (exp->val.primaryExp.primary_exp_val.constantExp.token_type == KW_TRUE)
				printf("value: true\n");
			else
				printf("value: false\n");
			break;
		}
		case call_exp:
		{
			int index = 0;
			EXP_node_list* head = exp->val.primaryExp.primary_exp_val.callExp.arguments;
			printf("%*s", level, "");
			printf("kind: ");
			level = level + 5;
			printf("FunctionCall:\n");
			printf("%*s", level, "");
			printf("function_name: %s\n", exp->val.primaryExp.primary_exp_val.callExp.name.value.str_val);
			printf("%*s", level, "");
			printf("arguments: Expressions\n");
			while(head != NULL) {
				printf("%*s", level+5, "");
				printf("expression[%d]: ", index++);
				print_expr(head->elem, level+10);
				head = head->next;
			}
			break;
		}
		default:
		{
			lerr("Wrong primary-expr type!", 1);
		}
	}
}

void print_stmt(STMT_node* s, int ident_level) {
	int level = ident_level;
	switch(s->kind) {
		case var_decl:
		{
			printf("StatementVarDecl:\n");
			printf("%*s", level, "");
			printf("name: %s\n", s->val.varDecl.name.value.str_val);
			printf("%*s", level, "");
			printf("type: ");
			print_type(s->val.varDecl.type, level);
			if (s->val.varDecl.decl_val != NULL) {
				printf("%*s", level, "");
				printf("value: ");
				print_expr(s->val.varDecl.decl_val, level);
			}
			break;
		}
		case input:
		{
			int index = 0;
			printf("StatementInput:\n");
			while(s->val.inputStmt.idents[index].value.str_val != NULL) {
				printf("%*s", level, "");
				printf("name[%d]: %s\n", index, s->val.inputStmt.idents[index].value.str_val);
				index++;
			}
			break;
		}
		case output:
		{
			int index = 0;
			EXP_node_list* head = s->val.outputStmt.expressions;
			printf("StatementOutput:\n");
			while(head != NULL) {
				printf("%*s", level, "");
				printf("expression[%d]: ", index++);
				print_expr(head->elem, level+5);
				head = head->next;
			}
			break;
		}
		case loop_for:
		{
			STMT_node_list* head = s->val.loopFor.body;
			int index = 0;
			printf("StatementLoopFor:\n");
			printf("%*s", level, "");
			printf("init: ");
			print_expr(s->val.loopFor.init, level+5);
			printf("%*s", level, "");
			printf("cond: ");
			print_expr(s->val.loopFor.cond, level+5);
			printf("%*s", level, "");
			printf("inc: ");
			print_expr(s->val.loopFor.inc, level+5);
			printf("%*s", level, "");
			printf("body: Statements:\n");
			while (head != NULL) {
				printf("%*s", level, "");
				printf("statement[%d]: ", index++);
				print_stmt(head->elem, level+5);
				head = head->next;
			}
			break;
		}
		case loop_for_in:
		{
			EXP_node_list* exp_head = s->val.loopForIn.expressions;
			STMT_node_list* stmt_head = s->val.loopForIn.body;
			char* str = s->val.loopForIn.name.value.str_val;
			int index = 0;
			printf("StatementLoopForIn:\n");
			printf("%*s", level, "");
			printf("ident: %s\n", str);
			printf("%*s", level, "");
			printf("list: Expressions:\n");
			while(exp_head != NULL) {
				printf("%*s", level+5, "");
				printf("expression[%d]: ", index++);
				print_expr(exp_head->elem, level+10);
				exp_head = exp_head->next;
			}
			printf("%*s", level, "");
			printf("body: Statements:\n");
			index = 0;
			while(stmt_head != NULL) {
				printf("%*s", level+5, "");
				printf("statement[%d]: ", index++);
				print_stmt(stmt_head->elem, level+10);
				stmt_head = stmt_head->next;
			}
			break;
		}
		case loop_while:
		{
			STMT_node_list* head = s->val.loopWhile.body;
			int index = 0;
			printf("StatementLoopWhile:\n");
			printf("%*s", level, "");
			printf("cond: ");
			print_expr(s->val.loopWhile.cond, level+5);
			printf("%*s", level, "");
			printf("body: Statements:\n");
			while (head != NULL) {
				printf("%*s", level+5, "");
				printf("statement[%d]: ", index++);
				print_stmt(head->elem, level+10);
				head = head->next;
			}
			break;
		}
		case loop_do_while:
		{
			STMT_node_list* head = s->val.loopDoWhile.body;
			int index = 0;
			printf("StatementLoopDoWhile:\n");
			printf("%*s", level, "");
			printf("cond: ");
			print_expr(s->val.loopDoWhile.cond, level+5);
			printf("%*s", level, "");
			printf("body: Statements:\n");
			while (head != NULL) {
				printf("%*s", level, "");
				printf("statement[%d]: ", index++);
				print_stmt(head->elem, level+5);
				head = head->next;
			}
			break;
		}
		case statement_if:
		{
			BRANCH_node_list* branches = s->val.stmtIf.branches;
			STMT_node_list* else_body = s->val.stmtIf.else_body;
			int index = 0;
			int i = 0;

			printf("StatementIf:\n");
			while (branches != NULL) {
				i = 0;
				printf("%*s", level+5, "");
				if (index == 0)
					printf("branch[%d]: IfBranch:\n", index++);
				else
					printf("branch[%d]: ElseIfBranch:\n", index++);
				printf("%*s", level+10, "");
				printf("cond: ");
				print_expr(branches->elem->cond, level+15);
				printf("%*s", level+10, "");
				printf("body: Statements:\n");
				while (branches->elem->body != NULL) {
					printf("%*s", level+15, "");
					printf("statement[%d]: ", i++);
					print_stmt(branches->elem->body->elem, level+20);
					branches->elem->body = branches->elem->body->next;
				}
				branches = branches->next;
			}
			if (else_body != NULL) {
				index = 0;
				printf("%*s", level+5, "");
				printf("else_body: Statements:\n");
				while (else_body != NULL) {
					printf("%*s", level+10, "");
					printf("statement[%d]: ", index++);
					print_stmt(else_body->elem, level+15);
					else_body = else_body->next;
				}
			}
			break;
		}
		case continue_stmt:
		{
			printf("StatementContinue\n");
			break;
		}
		case break_stmt:
		{
			printf("StatementBreak\n");
			break;
		}
		case return_stmt:
		{
			EXP_node* expr = s->val.stmtReturn.expression;
			printf("StatementReturn:\n");
			if (expr != NULL) {
				printf("%*s", level, "");
				print_expr(expr, level+5);
			}
			break;
		}
		case expr_stmt:
		{
			printf("ExprStatement\n");
			printf("%*s", level, "");
			print_expr(s->val.exprStmt.expression, level+5);
			break;
		}
		default:
		{
			lerr("Wrong statement type!", 1);
		}
	}
}

void print_param(PARAM_node* p) {
	int level = 25;
	PARAM_node* param = p;
	printf("%*s", level, "");
	printf("name: %s\n",param->name.value.str_val);
	printf("%*s", level, "");
	printf("type: ");
	print_type(param->type, level);
}

void print_function(FNDECL_node* func) {
	int level = 15;
	PARAM_node_list* parameters_head = func->params;
	STMT_node_list* statements_head = func->body;
	int index = 0;
	printf("%*s", level, "");
	printf("ret_type: ");
	print_type(func->ret_type, level);
	printf("%*s", level, "");
	printf("name: %s\n", func->func_name.value.str_val);
	printf("%*s", level, "");
	printf("params: Params:\n");
	while (parameters_head != NULL) {
		printf("%*s", level+5, "");
		printf("params[%d]: Param:\n", index++);
		print_param(parameters_head->elem);
		parameters_head = parameters_head->next;
	}
	index = 0;
	printf("%*s", level, "");
	printf("body: Statements:\n");
	while (statements_head != NULL) {
		printf("%*s", level+5, "");
		printf("statement[%d]: ", index++);
		print_stmt(statements_head->elem, level+10);
		statements_head = statements_head->next;
	}
}

void print_program(PROGRAM_node* program) {
	int index = 0;
	int level = 10;
	FUNC_node_list* head = program->functions;
	printf("Printing program:\n");
	printf("Program: Functions:\n");
	if (head != NULL)
		printf("%*s", level, "");
	while (head != NULL) {
		printf("function[%d]: FunctionDef:\n", index++);
		print_function(head->elem);
		head = head->next;
		if(head != NULL) {
			printf("%*s", level, "");
		}
	}
}
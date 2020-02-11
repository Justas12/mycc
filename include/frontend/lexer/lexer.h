#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include "safe-ctype.h"
#include "token.h"

#define NOTHING 1	// ignore chars in comment
#define PLUS 0X2B
#define MINUS 0X2D
#define ASTERISK 0X2A
#define SLASH 0X2F
#define PERC 0X25
#define EQUAL 0X3D
#define GREATER 0X3E
#define LESSER 0X3C
#define NOT 0X21
#define AND 0X26
#define OR 0X7C
#define BREACE_OPEN 0X7B
#define BREACE_CLOSE 0X7D
#define PAREN_OPEN 0X28
#define PAREN_CLOSE 0X29
#define BRACKET_OPEN 0X5B
#define BRACKET_CLOSE 0X5D
#define BACKSLASH 0X5C
#define COMMA 0X2C
#define COLON 0X3A
#define SEMICOLON 0X3B
#define DOT 0X2E
#define DQUOTE 0X22
#define NEW_LINE 0X0A
#define EXP 0X65
#define HORIZONTAL_TAB 0X09
#define WARNING 999
#define ERROR 1000

extern char* KEYWORD[];

typedef enum state {
	start,
	err,
	fatal__err,
	ident,
	lit_int,
	lit_str,
	lit_str_esc,
	lit_pre_float,
	lit_float,
	lit_float_exp,
	lit_float_exp_sign,
	lit_float_digits_only,
	comment_sl,
	comment_ml_end,
	comment_ml,
	op_ls,
	op_gr,
	op_lse,
	op_gre,
	op_nt,
	op_neq,
	op_aeq,
	op_ceq,
	op_land,
	op_band,
	op_lor,
	op_bor,
	op_div,
	op_plus,
	op_minus,
	op_mult,
	op_perc,
	op_breace_open,
	op_breace_close,
	op_paren_open,
	op_paren_close,
	op_bracket_open,
	op_bracket_close,
	op_backslash,
	op_comma,
	op_colon,
	op_semicolon,
	unknown,
	_eof
} State;

typedef struct buffer {
	char* data;
	int index;
} Buffer;

typedef struct status {
	int state;
	int offset_increment;
	int line_no;
} Status;

Token* get_tokens();
void print_tokens();
void init_globals(int);
int add_to_buffer(char);
void clear_buffer();
void print_buffer();
int set_status(int, int, int);
void clear_status();
void clear_buffer_and_status();
void complete_token(int, char*, int);
void make__error(char*, int, char*, int);
void handle_other_chars(int);
void lex_start(char);
void complete_ident();
void complete_int();
void complete_float();
void complete_string();
void complete_assign_div();
void complete_div();
void end_ml_comm();
int _err(char, int);
void lex_op_div(char);
void lex_comment_sl(char);
void lex_comment_ml(char);
void lex_comment_ml_end(char);
void lex_ident(char);
void lex_lit_int(char);
void lex_lit_float(char);
void lex_lit_float_exp(char);
void lex_lit_float_exp_sign(char);
void lex_lit_float_digits_only(char);
void lex_pre_lit_float(char);
void lex_lit_str(char);
void lex_lit_str_esc(char);
void complete_less_equal();
void complete_lesser();
void complete_great_equal();
void complete_greater();
void complete_comparison_equal();
void complete_assign_equal();
void complete_not_equal();
void complete_not();
void complete_logical_and();
void complete_bitwise_and();
void complete_logical_or();
void complete_bitwise_or();
void complete_assign_mult();
void complete_assign_minus();
void complete_assign_plus();
void complete_minus();
void complete_plus();
void complete_mult();
void lex_op_less(char);
void lex_op_great(char);
void lex_op_eq(char);
void lex_op_not(char);
void lex_op_a(char);
void lex_op_o(char);
void lex_op_mult(char);
void lex_op_plus(char);
void lex_op_minus(char);
int scan_text(const char*);
char* get_text(const char*);

#endif /* LEXER_H */
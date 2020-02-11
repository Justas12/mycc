#ifndef TOKEN_H
#define TOKEN_H
#include <stddef.h>

typedef enum token_type {
	KW_FOR,
	KW_IN,
	KW_DO,
	KW_WHILE,
	KW_IF,
	KW_ELSE,
	KW_RETURN,
	KW_VOID,
	KW_INT,
	KW_FLOAT,
	KW_BOOLEAN,
	KW_STRING,
	KW_BREAK,
	KW_CONTINUE,
	KW_READ,
	KW_WRITE,
	KW_FALSE,
	KW_TRUE,
	IDENT,
	LIT_INT,
	LIT_STR,
	LIT_FLOAT,
	OP_LS,
	OP_GR,
	OP_LSE,
	OP_GRE,
	OP_NT,
	OP_NEQ,
	OP_AEQ,
	OP_CEQ,
	OP_LAND,
	OP_BAND,
	OP_LOR,
	OP_BOR,
	OP_DIV,
	OP_PLUS,
	OP_MINUS,
	OP_MULT,
	OP_PERC,
	OP_BREACE_OPEN,
	OP_BREACE_CLOSE,
	OP_PAREN_OPEN,
	OP_PAREN_CLOSE,
	OP_BRACKET_OPEN,
	OP_BRACKET_CLOSE,
	OP_BACKSLASH,
	OP_COMMA,
	OP_COLON,
	OP_SEMICOLON,
	OP_MULEQ,
	OP_PLEQ,
	OP_MINEQ,
	OP_DIVEQ,
	_EOF,
	UNKNOWN
} Token_type;

typedef union value {
	int int_val;
	float float_val;
	char* str_val;
} Value;

typedef struct {
	int token_type;
	Value value;
	int line_no;
} Token;

#endif /* TOKEN_H */
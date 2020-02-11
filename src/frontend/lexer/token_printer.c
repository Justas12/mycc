#include "../../../include/frontend/lexer/token_printer.h"

const char* token_type_string[] = {
	"KW_FOR",
	"KW_IN",
	"KW_DO",
	"KW_WHILE",
	"KW_IF",
	"KW_ELSE",
	"KW_RETURN",
	"KW_VOID",
	"KW_INT",
	"KW_FLOAT",
	"KW_BOOLEAN",
	"KW_STRING",
	"KW_BREAK",
	"KW_CONTINUE",
	"KW_READ",
	"KW_WRITE",
	"KW_FALSE",
	"KW_TRUE",
	"IDENT",
	"LIT_INT",
	"LIT_STR",
	"LIT_FLOAT",
	"OP_LS",
	"OP_GR",
	"OP_LSE",
	"OP_GRE",
	"OP_NT",
	"OP_NEQ",
	"OP_AEQ",
	"OP_CEQ",
	"OP_LAND",
	"OP_BAND",
	"OP_LOR",
	"OP_BOR",
	"OP_DIV",
	"OP_PLUS",
	"OP_MINUS",
	"OP_MULT",
	"OP_PERC",
	"OP_BREACE_OPEN",
	"OP_BREACE_CLOSE",
	"OP_PAREN_OPEN",
	"OP_PAREN_CLOSE",
	"OP_BRACKET_OPEN",
	"OP_BRACKET_CLOSE",
	"OP_BACKSLASH",
	"OP_COMMA",
	"OP_COLON",
	"OP_SEMICOLON",
	"OP_MULEQ",
	"OP_PLEQ",
	"OP_MINEQ",
	"OP_DIVEQ",
	"EOF",
	"UNKNOWN",
	NULL
};

const char* token_type_symbol[] = {
	"KW_FOR",
	"KW_IN",
	"KW_DO",
	"KW_WHILE",
	"KW_IF",
	"KW_ELSE",
	"KW_RETURN",
	"KW_VOID",
	"KW_INT",
	"KW_FLOAT",
	"KW_BOOLEAN",
	"KW_STRING",
	"KW_BREAK",
	"KW_CONTINUE",
	"KW_READ",
	"KW_WRITE",
	"KW_FALSE",
	"KW_TRUE",
	"IDENT",
	"LIT_INT",
	"LIT_STR",
	"LIT_FLOAT",
	"<",
	">",
	"<=",
	">=",
	"!",
	"!=",
	"=",
	"==",
	"&&",
	"&",
	"||",
	"|",
	"/",
	"+",
	"-",
	"*",
	"%",
	"{",
	"}",
	"(",
	")",
	"[",
	"]",
	"\\",
	",",
	":",
	";",
	"*=",
	"+=",
	"-=",
	"/=",
	"EOF",
	"UNKNOWN",
	NULL
};

void print_tokens(Token* tokens) {
	int i;
	char* type = malloc(sizeof(char) * 30);
	printf("%-5s%-6s%-17s%-50s\n", "ID", "|LN", "|TYPE", "|VALUE");
	printf("-----+-----+----------------+------------------\n");
	
	for (i = 0; tokens[i].token_type != _EOF; ++i) {
		strcpy(type, token_type_string[tokens[i].token_type]);
		printf("%-5d", i);
		printf("|");
		printf("%-5d", tokens[i].line_no);
		printf("|");
		printf("%-16s", type);
		printf("|");
		if (tokens[i].token_type == LIT_INT)
			printf("%-15d", tokens[i].value.int_val);
		else if (tokens[i].token_type == LIT_FLOAT)
			printf("%-15.2f", tokens[i].value.float_val);
		else if (tokens[i].token_type == LIT_STR || tokens[i].token_type == IDENT)
			printf("%-15s", tokens[i].value.str_val);
		printf("\n");
	}
	printf("%-5d", i);
	printf("|");
	printf("%-5d", tokens[i].line_no);
	printf("|");
	printf("%-16s", token_type_string[tokens[i].token_type]);
	printf("|");
	printf("\n");
}
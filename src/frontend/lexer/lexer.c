#include "../../../include/frontend/lexer/lexer.h"
#include "../../../include/utils/utils.h"

char* KEYWORD[] = {	
	"for",
	"in",
	"do",
	"while",
	"if",
	"else",
	"return",
	"void",
	"int",
	"float",
	"boolean",
	"string",
	"break",
	"continue",
	"read",
	"write",
	"false",
	"true",
	NULL
};

Buffer* buff;
Status* status;
Token* tokens;

Token* get_tokens() {
	return tokens;
}

void init_globals(int size) {
	tokens = malloc(size * sizeof(Token));
	buff = malloc(sizeof(Buffer));
	buff->data = malloc(size * sizeof(char));
	buff->index = 0;
	status = malloc(sizeof(Status));
	status->state = start;
	status->offset_increment = 0;
	status->line_no = 1;
}

int add_to_buffer(char c) {
	int index = buff->index;
	buff->data[index] = c;
	buff->index = buff->index + 1;
	return 1;
}

void clear_buffer() {
	int length = buff->index;
	for (int i = 0; i < length; ++i)
		buff->data[i] = 0x00;
	buff->index = 0;
}

void print_buffer() {
	int length = buff->index;
	for (int i = 0; i < length; ++i)
		printf("%c", buff->data[i]);
}

int set_status(int state, int offset, int newline) {
	status->state = state;
	status->offset_increment = offset;
	status->line_no = status->line_no + newline;
	return 1;
}

void clear_status() {
	status->state = start;
	status->offset_increment = 0;
}

void clear_buffer_and_status() {
	clear_buffer();
	clear_status();
}

void complete_token(int type, char* value, int line_no) {
	static int size = 0;
	if (type == LIT_INT)
		tokens[size++] = make_int_token(atoi(value), line_no);
	else if (type == LIT_FLOAT)
		tokens[size++] = make_float_token(atof(value), line_no);
	else
		tokens[size++] = make_token(type, value, line_no);
}

void make__error(char* buff, int line_no, char* msg, int _error) {
	if (_error == ERROR) {
		printf("Stopped Scanning!\n");
		printf("%s:%d %s: %s: \'%s\'\n", filename, line_no, "error", msg, buff);	
		set_status(fatal__err, 0, 0);
	}
	else
		printf("%s:%d %s: %s: \'%s\'\n", filename, line_no, "warning", msg, buff);
}

void handle_other_chars(int c) {
	if (c == PERC) {
		complete_token(OP_PERC, NULL, status->line_no);
	}
	else if (c == BREACE_OPEN) {
		complete_token(OP_BREACE_OPEN, NULL, status->line_no);
	}
	else if (c == BREACE_CLOSE) {
		complete_token(OP_BREACE_CLOSE, NULL, status->line_no);
	}
	else if (c == PAREN_OPEN) {
		complete_token(OP_PAREN_OPEN, NULL, status->line_no);
	}
	else if (c == PAREN_CLOSE) {
		complete_token(OP_PAREN_CLOSE, NULL, status->line_no);
	}
	else if (c == BRACKET_OPEN) {
		complete_token(OP_BRACKET_OPEN, NULL, status->line_no);
	}
	else if (c == BRACKET_CLOSE) {
		complete_token(OP_BRACKET_CLOSE, NULL, status->line_no);
	}
	else if (c == COMMA) {
		complete_token(OP_COMMA, NULL, status->line_no);
	}
	else if (c == COLON) {
		complete_token(OP_COLON, NULL, status->line_no);
	}
	else if (c == SEMICOLON) {
		complete_token(OP_SEMICOLON, NULL, status->line_no);
	}
	else
		complete_token(UNKNOWN, NULL, status->line_no);
}

void lex_start(char c) {
	if(ISIDST(c)) {	/* A-Za-z_ */
		add_to_buffer(c);
		set_status(ident, 1, 0);
	}
	else if(ISDIGIT(c)) {	/* 0-9 */
		add_to_buffer(c);
		set_status(lit_int, 1, 0);
	}
	else if(ISSPACE(c)) {	/* space tab \n \r \f \v */
		if(c == NEW_LINE)	/* \n */
			set_status(start, 1, 1);
		else
			set_status(start, 1, 0);
	}
	else {
		switch (c) {
			case DOT:
			{
				add_to_buffer(c);
				set_status(lit_pre_float, 1, 0);
			}break;
			case DQUOTE:
			{
				set_status(lit_str, 1, 0);
			}break;
			case SLASH:
			{
				add_to_buffer(c);
				set_status(op_div, 1, 0);
			}break;
			case EQUAL:
			{
				add_to_buffer(c);
				set_status(op_aeq, 1, 0);
			}break;
			case LESSER:
			{
				add_to_buffer(c);
				set_status(op_ls, 1, 0);
			}break;
			case GREATER:
			{
				add_to_buffer(c);
				set_status(op_gr, 1, 0);
			}break;
			case NOT:
			{
				add_to_buffer(c);
				set_status(op_nt, 1, 0);
			}break;
			case AND:
			{
				add_to_buffer(c);
				set_status(op_band, 1, 0);
			}break;
			case OR:
			{
				add_to_buffer(c);
				set_status(op_bor, 1, 0);
			}break;
			case PLUS:
			{
				add_to_buffer(c);
				set_status(op_plus, 1, 0);
			}break;
			case MINUS:
			{
				add_to_buffer(c);
				set_status(op_minus, 1, 0);
			}break;
			case ASTERISK:
			{
				add_to_buffer(c);
				set_status(op_mult, 1, 0);
			}break;
			default:
			{
				handle_other_chars(c);
				set_status(start, 1, 0);
			}
		}
	}
}

void complete_ident() {
	int j;
	for (j = 0; KEYWORD[j]!=NULL; ++j) {
		if (strcmp(KEYWORD[j], buff->data) == 0) {
			complete_token(j, NULL, status->line_no);
			clear_buffer_and_status();
			return;
		}
	}
	complete_token(IDENT, buff->data, status->line_no);
	clear_buffer_and_status();
}

void complete_int() {
	complete_token(LIT_INT, buff->data, status->line_no);
	clear_buffer_and_status();
}

void complete_float() {
	complete_token(LIT_FLOAT, buff->data, status->line_no);
	clear_buffer_and_status();
}

void complete_string() {
	complete_token(LIT_STR, buff->data, status->line_no);
	clear_buffer();
	set_status(start, 1, 0);
}

void complete_assign_div() {
	complete_token(OP_DIVEQ, NULL, status->line_no);
	clear_buffer();
	set_status(start, 1, 0);
}

void complete_div() {
	complete_token(OP_DIV, buff->data, status->line_no);
	clear_buffer_and_status();
}

void end_ml_comm() {
	clear_buffer();
	set_status(start, 1, 0);
}

int _err(char c, int state) {
	static Token_type s;
	if (state == lit_int)
		s = LIT_INT;
	else if (state == lit_pre_float
				|| state == lit_float
		  		|| state == lit_float_exp
		  		|| state == lit_float_exp_sign
		  		|| state == lit_float_digits_only)
		s = LIT_FLOAT;
	set_status(err, 1, 0);
	if (s == LIT_INT) {
		(!ISIDNUM(c) || c == 0x00) ? make__error(buff->data, status->line_no, "invalid integer format", ERROR) : add_to_buffer(c);
	}
	else 
		(!ISIDNUM(c) || c == 0x00) ? make__error(buff->data, status->line_no, "invalid float format", ERROR) : add_to_buffer(c);
	return 1;
}

void lex_op_div(char c) {
	c == SLASH ? set_status(comment_sl, 1, 0) : (c == ASTERISK ? set_status(comment_ml, 1, 0) : (c == EQUAL ? complete_assign_div() : complete_div()));
}

void lex_comment_sl(char c) {
	c == NEW_LINE ? clear_buffer_and_status() : NOTHING;
}

void lex_comment_ml(char c) {
	c == ASTERISK ? set_status(comment_ml_end, 1, 0) : (c == NEW_LINE ? set_status(comment_ml, 1, 1) : NOTHING);
}

void lex_comment_ml_end(char c) {
	c == SLASH ? end_ml_comm() : (c == NEW_LINE ? set_status(comment_ml, 1, 1) : (c == ASTERISK ? NOTHING : set_status(comment_ml, 1, 0)));
}

void lex_ident(char c) {
	ISIDNUM(c) ? add_to_buffer(c) : complete_ident();
}

void lex_lit_int(char c) {
	ISDIGIT(c) ? add_to_buffer(c) : (c == DOT ? add_to_buffer(c) && set_status(lit_float, 1, 0) : (c == EXP ? add_to_buffer(c) && set_status(lit_float_exp, 1, 0) : (!ISIDST(c) ? complete_int() : _err(c, status->state))));
}

void lex_lit_float(char c) {
	ISDIGIT(c) ? add_to_buffer(c) : (c == EXP ? add_to_buffer(c) && set_status(lit_float_exp, 1, 0) : (!ISIDST(c) ? complete_float() : _err(c, status->state)));
}

void lex_lit_float_exp(char c) {
	ISDIGIT(c) ? set_status(lit_float_digits_only, 1, 0) : (c == PLUS || c == MINUS ? add_to_buffer(c) && set_status(lit_float_exp_sign, 1, 0) : _err(c, status->state));
}

void lex_lit_float_exp_sign(char c) {
	ISDIGIT(c) ? add_to_buffer(c) && set_status(lit_float_digits_only, 1, 0) : _err(c, status->state);
}

void lex_lit_float_digits_only(char c) {
	ISDIGIT(c) ? add_to_buffer(c) : (!ISIDST(c) ? complete_float() : _err(c, status->state));
}

void lex_pre_lit_float(char c) {
	ISDIGIT(c) ? add_to_buffer(c) && set_status(lit_float, 1, 0) : _err(c, status->state);
}

void lex_lit_str(char c) {
	c == DQUOTE ? complete_string() : (c == BACKSLASH ? set_status(lit_str_esc, 1, 0) : (c == NEW_LINE ? make__error(buff->data, status->line_no, "unterminated string", ERROR) : add_to_buffer(c)));
}

void lex_lit_str_esc(char c) {
	if (c == DQUOTE) {
		add_to_buffer(DQUOTE);
	}
	else if (c == 'n') {
		add_to_buffer(NEW_LINE);
	}
	else if (c == 't') {
		add_to_buffer(HORIZONTAL_TAB);
	}
	else if (c == '\\') {
		add_to_buffer(BACKSLASH);
	}
	else if (ISSPACE(c)) {
		make__error(buff->data, status->line_no, "unterminated string", ERROR);
		return;
	}
	else {
		add_to_buffer(c);
		char* str = malloc(100);
		sprintf(str, "unknown escpe sequence: %s%c", "\\", c);
		make__error(buff->data, status->line_no, str, WARNING);
	}
	set_status(lit_str, 1, 0);
}

void complete_less_equal() {
	complete_token(OP_LSE, NULL, status->line_no);
	clear_buffer();
	set_status(start, 1, 0);
}

void complete_lesser() {
	complete_token(OP_LS, NULL, status->line_no);
	clear_buffer_and_status();
}

void complete_great_equal() {
	complete_token(OP_GRE, NULL, status->line_no);
	clear_buffer();
	set_status(start, 1, 0);
}

void complete_greater() {
	complete_token(OP_GR, NULL, status->line_no);
	clear_buffer_and_status();
}

void complete_comparison_equal() {
	complete_token(OP_CEQ, NULL, status->line_no);
	clear_buffer();
	set_status(start, 1, 0);
}

void complete_assign_equal() {
	complete_token(OP_AEQ, NULL, status->line_no);
	clear_buffer_and_status();
}

void complete_not_equal() {
	complete_token(OP_NEQ, NULL, status->line_no);
	clear_buffer();
	set_status(start, 1, 0);
}

void complete_not() {
	complete_token(OP_NT, NULL, status->line_no);
	clear_buffer_and_status();
}

void complete_logical_and() {
	complete_token(OP_LAND, NULL, status->line_no);
	clear_buffer();
	set_status(start, 1, 0);
}

void complete_bitwise_and() {
	complete_token(OP_BAND, NULL, status->line_no);
	clear_buffer_and_status();
}

void complete_logical_or() {
	complete_token(OP_LOR, NULL, status->line_no);
	clear_buffer();
	set_status(start, 1, 0);
}

void complete_bitwise_or() {
	complete_token(OP_BOR, NULL, status->line_no);
	clear_buffer_and_status();
}

void complete_assign_mult() {
	complete_token(OP_MULEQ, NULL, status->line_no);
	clear_buffer();
	set_status(start, 1, 0);
}

void complete_assign_minus() {
	complete_token(OP_MINEQ, NULL, status->line_no);
	clear_buffer();
	set_status(start, 1, 0);
}

void complete_assign_plus() {
	complete_token(OP_PLEQ, NULL, status->line_no);
	clear_buffer();
	set_status(start, 1, 0);
}

void complete_minus() {
	complete_token(OP_MINUS, NULL, status->line_no);
	clear_buffer_and_status();
}

void complete_plus() {
	complete_token(OP_PLUS, NULL, status->line_no);
	clear_buffer_and_status();
}

void complete_mult() {
	complete_token(OP_MULT, NULL, status->line_no);
	clear_buffer_and_status();
}

void lex_op_less(char c) {
	c == EQUAL ? complete_less_equal() : complete_lesser();
}
void lex_op_great(char c) {
	c == EQUAL ? complete_great_equal() : complete_greater();
}
void lex_op_eq(char c) {
	c == EQUAL ? complete_comparison_equal() : complete_assign_equal();
}
void lex_op_not(char c) {
	c == EQUAL ? complete_not_equal() : complete_not();
}
void lex_op_a(char c) {
	c == AND ? complete_logical_and() : complete_bitwise_and();
}
void lex_op_o(char c) {
	c == OR ? complete_logical_or() : complete_bitwise_or();
}

void lex_op_mult(char c) {
	c == EQUAL ? complete_assign_mult() : complete_mult();
}

void lex_op_plus(char c) {
	c == EQUAL ? complete_assign_plus() : complete_plus();
}

void lex_op_minus(char c) {
	c == EQUAL ? complete_assign_minus() : complete_minus();
}

char* get_text(const char* filename) {
	int fd;
	ssize_t sz;
	struct stat st;
	char* text;

	if ((fd = open(filename, O_RDONLY, 0644)) == -1)
		perr("open");
	stat(filename, &st);
	sz = st.st_size;
	init_globals(sz);
	text = malloc(sizeof(char)*sz);
	if (read(fd, text, sz) == -1)
		perr("read");
	return text;
}

int scan_text(const char* filename) {
	int offset;
	char c;
	char* text = get_text(filename);
	int n = strlen(text) + 1;
	for (offset=0; offset < n; offset=offset+status->offset_increment) {
		c = text[offset];
		if (c == 0x00) {
			if (status->state != start) {
				if (status->state == err) {
					_err(c, err);
					return -1;
				}
				else if (status->state == lit_str) {
					make__error(buff->data, status->line_no, "unterminated string", ERROR);
					return -1;
				}
				else if (status->state == comment_ml || status->state == comment_ml_end) {
					make__error(buff->data, status->line_no, "unterminated comment", ERROR);
					return -1;
				}
				else if (status->state == ident) {
					complete_ident();
				}
				else if (status->state == lit_pre_float
							|| status->state == lit_float
					  		|| status->state == lit_float_exp
					  		|| status->state == lit_float_exp_sign
					  		|| status->state == lit_float_digits_only) {
					complete_float();
				}
				else if (status->state == lit_int) {
					complete_int();
				}
			}
				complete_token(_EOF, NULL, status->line_no);
				break;
		}
		else {
			switch (status->state) {
				case start:
					lex_start(c);  break;
				case comment_sl:
					 lex_comment_sl(c); break;
				case comment_ml:
					lex_comment_ml(c); break;
				case comment_ml_end:
					lex_comment_ml_end(c); break;
				case ident:
					lex_ident(c); break;
				case lit_int:
					lex_lit_int(c); break;
				case lit_float:
					lex_lit_float(c); break;
				case lit_pre_float:
					lex_pre_lit_float(c); break;
				case lit_float_exp:
					lex_lit_float_exp(c); break;
				case lit_float_exp_sign:
					lex_lit_float_exp_sign(c); break;
				case lit_float_digits_only:
					lex_lit_float_digits_only(c); break;
				case lit_str:
					lex_lit_str(c); break;
				case lit_str_esc:
					lex_lit_str_esc(c); break;
				case op_ls:
					lex_op_less(c); break;
				case op_gr:
					lex_op_great(c); break;
				case op_aeq:
					lex_op_eq(c); break;
				case op_nt:
					lex_op_not(c); break;
				case op_band:
					lex_op_a(c); break;
				case op_bor:
					lex_op_o(c); break;
				case op_div:
					lex_op_div(c); break;
				case op_minus:
					lex_op_minus(c); break;
				case op_plus:
					lex_op_plus(c); break;
				case op_mult:
					lex_op_mult(c); break;
				case err:
					_err(c, err); break;
				case fatal__err:
					return -1;
				default: printf("%s\n", "BAD STATE!"); return -1;
			}
		}
	}
	return 0;
}

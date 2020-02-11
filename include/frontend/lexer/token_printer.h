#ifndef TOKEN_PRINTER_H
#define TOKEN_PRINTER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token.h"

extern const char* token_type_string[];
extern const char* token_type_symbol[];

void print_tokens(Token*);

#endif /* TOKEN_PRINTER_H */
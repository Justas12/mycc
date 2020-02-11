#include <stdlib.h>
#include <string.h>
#include "../../../include/frontend/symbol/symbol.h"

int Hash(char* str) {
	unsigned int hash = 0;
	while (*str) hash = (hash << 1) + *str++;
	return hash % hash_size;
}

Symbol_table* init_symbol_table() {
	Symbol_table* t;
	int i;
	t = (Symbol_table*)malloc(sizeof(Symbol_table));
	for (i = 0; i < hash_size; ++i)
		t->table[i] = NULL;
	t->next = NULL;
	t->scope_kind = NULL;
	return t;
}

Symbol_table* scope_symbol_table(Symbol_table* s, STMT_node* kind) {
	Symbol_table* t;
	t = init_symbol_table();
	t->next = s;
	t->scope_kind = kind;
	return t;
}

Symbol_table* unscope_symbol_table(Symbol_table* s) {
	return s->next;
}

Symbol* put_symbol(Symbol_table* t, Token key, int kind) {
	char* name = key.value.str_val;
	int i = Hash(name);
	Symbol* s;
	for (s = t->table[i]; s; s = s->next) {
		if (strcmp(s->key.value.str_val, name) == 0)
			return s;
	}
	s = (Symbol*)malloc(sizeof(Symbol));
	s->key = key;
	s->kind = kind;
	s->next = t->table[i];
	t->table[i] = s;
	return s;
}

Symbol* get_symbol(Symbol_table* t, Token key) {
	char* name = key.value.str_val;
	int i = Hash(name);
	Symbol* s;
	for (s = t->table[i]; s; s = s->next) {
		if (strcmp(s->key.value.str_val, name) == 0)
			return s;
	}
	if (t->next == NULL)
		return NULL;
	return get_symbol(t->next, key);
}

int def_symbol(Symbol_table* t, Token key) {
	char* name = key.value.str_val;
	int i = Hash(name);
	Symbol* s;
	for (s = t->table[i]; s; s = s->next) {
		if (strcmp(s->key.value.str_val, name) == 0)
			return 1;
	}
	return 0;
}
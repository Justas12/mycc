#include <stdio.h>

#include "../../include/frontend/lexer/lexer.h"
#include "../../include/frontend/parser/parser.h"
#include "../../include/frontend/ast/tree_printer.h"
#include "../../include/frontend/symbol/name_resolver.h"
#include "../../include/frontend/symbol/type_checker.h"

#include "../../include/backend/code/instrs.h"
#include "../../include/backend/code/code.h"
#include "../../include/backend/vm/vm.h"

#include "../../include/utils/utils.h"

int main(int argc, char const *argv[]) {
	Token* tokens;
	PROGRAM_node* root;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
	}
	filename = malloc(sizeof(char)*strlen(argv[1])+1);
	strcpy(filename, argv[1]);
	if (scan_text(filename) == -1)
		return 1;
	tokens = get_tokens();
	root = parse_program(tokens);
	if (error == 1)
		exit(1);
	sym_program(root);
	if (error == 1)
		exit(1);

	/* PRINTING */
	// print_tokens(tokens);
	// print_program(root);
	// print_code();

	/* FETCHING CODE INSTRUCTIONS */
	int* code = get_code();

	/* CREATING VM INSTANCE AND RUNNING IT */
	VM* vm = newVM(code/* <-- INSTRUNCTIONS */, 0/*<-- STARTING POINT */, 512/* <-- DATASIZE */);
	run(vm);

	/* FREEING OCCUPIED MEMORY */
	free_memory(code);
	free_memory(filename);

	return 0;
}
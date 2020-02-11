#include "../../../include/backend/code/instrs.h"
#include "../../../include/utils/utils.h"

Instruction instructions[INSTRUCTIONS_SIZE] = {

	/* ARITHMETIC_INSTRUCTIONS */
	{ 0x00, 0, I_INT_ADD },
	{ 0x01, 0, I_INT_SUB },
	{ 0x02, 0, I_INT_MULT },
	{ 0x03, 0, I_INT_DIV },
	{ 0x04, 0, I_INT_MOD },

	{ 0x05, 0, I_FLOAT_ADD },
	{ 0x06, 0, I_FLOAT_SUB },
	{ 0x07, 0, I_FLOAT_MULT },
	{ 0x08, 0, I_FLOAT_DIV },
	{ 0x09, 0, I_FLOAT_MOD },


	/* COMPARISON_INSTRUCTIONS */
	{ 0x10, 0, I_INT_LESSER },
	{ 0x11, 0, I_INT_LESSEREQ },
	{ 0x12, 0, I_INT_GREATER },
	{ 0x13, 0, I_INT_GRESTEREQ },

	{ 0x16, 0, I_FLOAT_LESSER },
	{ 0x17, 0, I_FLOAT_LESSEREQ },
	{ 0x18, 0, I_FLOAT_GREATER },
	{ 0x19, 0, I_FLOAT_GREATEREQ },


	/* STACK_INSTRUCTIONS */
	{ 0x30, 0, I_POP },
	{ 0x31, 1, I_INT_PUSH },
	{ 0x32, 4, I_FLOAT_PUSH },
	{ 0x33, 1, I_STRING_PUSH },
	{ 0x34, 1, I_GET_L },
	{ 0x35, 1, I_SET_L },


	/* CONTROL_INSTRUCTIONS */
	{ 0x41, 1, I_BZ },
	{ 0x42, 0, I_RET },
	{ 0x43, 0, I_RET_V },
	{ 0x44, 0, I_CALL_BEGIN },
	{ 0x45, 2, I_CALL },
	{ 0x46, 0, I_EXIT },
	{ 0x47, 1, I_BR },


	/* LOGICAL_INSTRUCTIONS */
	{ 0x50, 0, I_AND },
	{ 0x51, 0, I_OR },
	{ 0x52, 0, I_NOT },


	/* EQUALITY_INSTRUCTIONS */
	{ 0x60, 0, I_INT_EQ },
	{ 0x61, 0, I_INT_NEQ },
	{ 0x62, 0, I_FLOAT_EQ },
	{ 0x63, 0, I_FLOAT_NEQ },
	{ 0x64, 0, I_STRING_EQ },
	{ 0x65, 0, I_STRING_NEQ },


	/* IO_INSTRUCTIONS */
	{ 0x70, 0, I_INT_PRINT },
	{ 0x71, 0, I_FLOAT_PRINT },
	{ 0x72, 0, I_STRING_PRINT },
	
	{ 0x73, 1, I_INT_READ },
	{ 0x74, 1, I_FLOAT_READ },
	{ 0x75, 1, I_STRING_READ },
};

const char* str_instructions[INSTRUCTIONS_SIZE] = {

		/* ARITHMETIC_INSTRUCTIONS */
		"I_INT_ADD",
		"I_INT_SUB",
		"I_INT_MULT",
		"I_INT_DIV",
		"I_INT_MOD",
		"I_FLOAT_ADD",
		"I_FLOAT_SUB",
		"I_FLOAT_MULT",
		"I_FLOAT_DIV",
		"I_FLOAT_MOD",

		/* COMPARISON_INSTRUCTIONS */
		"I_INT_LESSER",
		"I_INT_LESSEREQ",
		"I_INT_GREATER",
		"I_INT_GRESTEREQ",
		"I_FLOAT_LESSER",
		"I_FLOAT_LESSEREQ",
		"I_FLOAT_GREATER",
		"I_FLOAT_GREATEREQ",

		/* STACK_INSTRUCTIONS */
		"I_POP",
		"I_INT_PUSH",
		"I_FLOAT_PUSH",
		"I_STRING_PUSH",
		"I_GET_L",
		"I_SET_L",

		/* CONTROL_INSTRUCTIONS */
		"I_BZ",
		"I_RET",
		"I_RET_V",
		"I_CALL_BEGIN",
		"I_CALL",
		"I_EXIT",
		"I_BR",

		/* LOGICAL_INSTRUCTIONS */
		"I_AND",
		"I_OR",
		"I_NOT",

		/* EQUALITY_INSTRUCTIONS */
		"I_INT_EQ",
		"I_INT_NEQ",
		"I_FLOAT_EQ",
		"I_FLOAT_NEQ",
		"I_STRING_EQ",
		"I_STRING_NEQ",

		/* IO_INSTRUCTIONS */
		"I_INT_PRINT",
		"I_FLOAT_PRINT",
		"I_STRING_PRINT",
		
		"I_INT_READ",
		"I_FLOAT_READ",
		"I_STRING_READ",

};

Instruction get_instr_by_opcode(unsigned char code) {
	int i;
	for (i = 0; i < INSTRUCTIONS_SIZE; ++i) {
		if (instructions[i].op_code == code)
			return instructions[i];
	}
	return instructions[0];
	lerr("no such instruction", 10);
}

Instruction get_instr_by_name(int name) {
	int i;
	for (i = 0; i < INSTRUCTIONS_SIZE; ++i) {
		if (instructions[i].name == name)
			return instructions[i];
	}
	return instructions[0];
	lerr("no such instruction", 10);
}

int get_instr_index_by_name(int name) {
	int i;
	for (i = 0; i < INSTRUCTIONS_SIZE; ++i) {
		if (instructions[i].name == name)
			return i;
	}
	return -1;
	lerr("no such instruction", 10);
}
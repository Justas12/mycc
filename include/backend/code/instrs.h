#ifndef INSTRS_H
#define INSTRS_H
#define INSTRUCTIONS_SIZE 70	// !!!
#include "../../frontend/lexer/token.h"

typedef struct instr {
	unsigned char op_code;
	int num_ops;	
	enum {
		/* ARITHMETIC_INSTRUCTIONS */
		I_INT_ADD,			// (0)
		I_INT_SUB,			// (1)
		I_INT_MULT,			// (2)
		I_INT_DIV,			// (3)
		I_INT_MOD,			// (4)
		I_FLOAT_ADD,		// (5)
		I_FLOAT_SUB,		// (6)
		I_FLOAT_MULT,		// (7)
		I_FLOAT_DIV,		// (8)
		I_FLOAT_MOD,		// (9)

		/* COMPARISON_INSTRUCTIONS */
		I_INT_LESSER,		// (10)
		I_INT_LESSEREQ,		// (11)
		I_INT_GREATER,		// (12)
		I_INT_GRESTEREQ,	// (13)
		I_FLOAT_LESSER,		// (14)
		I_FLOAT_LESSEREQ,	// (15)
		I_FLOAT_GREATER,	// (16)
		I_FLOAT_GREATEREQ,	// (17)

		/* STACK_INSTRUCTIONS */
		I_POP,				// (18)
		I_INT_PUSH,			// (19)
		I_FLOAT_PUSH,		// (20) !!!
		I_STRING_PUSH,		// (22)
		I_GET_L,			// (23)	!!!
		I_SET_L,			// (26) !!!

		/* CONTROL_INSTRUCTIONS */
		I_BZ,				// (29)
		I_RET,				// (30)
		I_RET_V,			// (31)
		I_CALL_BEGIN,		// (32)
		I_CALL,				// (33)
		I_EXIT,				// (34)
		I_BR,				// (35)

		/* LOGICAL_INSTRUCTIONS */
		I_AND,				// (36)
		I_OR,				// (37)
		I_NOT,
		
		/* EQUALITY_INSTRUCTIONS */
		I_INT_EQ,			// (38)
		I_INT_NEQ,			// (39)
		I_FLOAT_EQ,			// (40)
		I_FLOAT_NEQ,		// (41)
		I_STRING_EQ,		// (42)
		I_STRING_NEQ,		// (43)

		/* IO_INSTRUCTIONS */
		I_INT_PRINT,		// (46)
		I_FLOAT_PRINT,		// (47)
		I_STRING_PRINT,		// (48)
		
		I_INT_READ,			// (49)
		I_FLOAT_READ,		// (50)
		I_STRING_READ,		// (51)

	} name;
} Instruction;

extern const char* str_instructions[INSTRUCTIONS_SIZE];
extern Instruction instructions[INSTRUCTIONS_SIZE];

Instruction get_instr_by_opcode(unsigned char);
Instruction get_instr_by_name(int);
int get_instr_index_by_name(int);

#endif /* INSTRS_H */

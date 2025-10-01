#pragma once

#include <stddef.h>

typedef struct {
	const char *name;
	size_t arg_count;
	int opcode;
} cpu_command;

const cpu_command cpu_commands[] = {
	// name, n_args, opcode
	{ "PUSH", 1, 1 },
	{ "ADD",  0, 2 },
	{ "SUB",  0, 3 },
	{ "MUL",  0, 4 },
	{ "DIV",  0, 5 },
	{ "SQRT", 0, 6 },
	{ "OUT",  0, 7 },
	{ "HLT",  0, 8 },
};

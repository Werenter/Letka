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

#define COMMAND_PUSH 1
#define COMMAND_ADD  2
#define COMMAND_SUB  3
#define COMMAND_MUL  4
#define COMMAND_DIV  5
#define COMMAND_SQRT 6
#define COMMAND_OUT  7
#define COMMAND_HLT  8

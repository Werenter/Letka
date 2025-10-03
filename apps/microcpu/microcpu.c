#include <stdio.h>
#include <string.h>
#include <math.h>

#include "stack.h"
#include "logger.h"
#include "sq_io.h"
#include "cpu_commands.h"

static void mainloop(StackInt *stack, int *instruction_data, size_t data_len);
static int read_input(const char *path, int **input_data, size_t *input_size);
static Status_type command_add(StackInt *stack);
static Status_type command_sub(StackInt *stack);
static Status_type command_mul(StackInt *stack);
static Status_type command_div(StackInt *stack);
static Status_type command_sqrt(StackInt *stack);
static Status_type command_push(StackInt *stack, int *instruction_data, size_t *inst_pos, size_t inst_size);

// Уж лучше макросы, чем копипаста
#define BINARY_OPERATION(stack, operation, name) {\
	if(stack->size < 2) {\
		colored_fprintf(CYAN, stdout, "Stack must have at least two elements for " name " command\n");\
		return STATUS_EMPTY;\
	}\
	\
	Status_type err1 = STATUS_OK;\
	Status_type err2 = STATUS_OK;\
	Status_type err3 = STATUS_OK;\
	int b = stack_int_pop(stack, &err1);\
	int a = stack_int_pop(stack, &err2);\
	stack_int_push(a operation b, stack, &err3);\
	if(err1 != STATUS_OK || err2 != STATUS_OK || err3 != STATUS_OK) {\
		LOG_ERROR("Internal stack error: %u %u %u\n", err1, err2, err3);\
		STACK_INT_DUMP(stack);\
	}\
}

// Если выпилить проверки, будет намного красивее, но увы,
// у нас типа всё надёжно...
static Status_type command_add(StackInt *stack) {
	BINARY_OPERATION(stack, +, "ADD");
	return STATUS_OK;
}

static Status_type command_sub(StackInt *stack) {
	BINARY_OPERATION(stack, -, "SUB");
	return STATUS_OK;
}

static Status_type command_mul(StackInt *stack) {
	BINARY_OPERATION(stack, *, "MUL");
	return STATUS_OK;
}

static Status_type command_div(StackInt *stack) {
	BINARY_OPERATION(stack, /, "DIV");
	return STATUS_OK;
}

static Status_type command_sqrt(StackInt *stack) {
	if(stack->size < 1) {
		colored_fprintf(CYAN, stdout, "Stack must have at least one element for SQRT command\n");
		return STATUS_EMPTY;
	}
	
	Status_type err1 = STATUS_OK;
	Status_type err2 = STATUS_OK;
	int a = stack_int_pop(stack, &err2);
	stack_int_push((int)sqrt((double)a), stack, &err2);
	if(err1 != STATUS_OK || err2 != STATUS_OK) {
		LOG_ERROR("Internal stack error: %u %u\n", err1, err2);
		STACK_INT_DUMP(stack);
	}
	return STATUS_OK;
}

static Status_type command_push(StackInt *stack, int *instruction_data, size_t *inst_pos, size_t inst_size) {
	Status_type err = STATUS_OK;
	if(*inst_pos+1 >= inst_size) return STATUS_EMPTY;

	stack_int_push(instruction_data[++(*inst_pos)], stack, &err);
	if(err != STATUS_OK) {
		LOG_ERROR("Internal stack error: %u\n", err);
		STACK_INT_DUMP(stack);
		return err;
	}
	return STATUS_OK;
}

void mainloop(StackInt *stack, int *instruction_data, size_t data_len) {
	Status_type err = STATUS_OK;
	for(size_t i = 0; i < data_len; i++) {
		switch(instruction_data[i]) {
			case COMMAND_PUSH:
				err = command_push(stack, instruction_data, &i, data_len);
				break;
			case COMMAND_MUL:
				err = command_mul(stack);
				break;
			case COMMAND_DIV:
				err = command_div(stack);
				break;
			case COMMAND_ADD:
				err = command_add(stack);
				break;
			case COMMAND_SUB:
				err = command_sub(stack);
				break;
			case COMMAND_SQRT:
				err = command_sqrt(stack);
				break;
			case COMMAND_OUT:
				print_stack(stack, stdout, &err);
				break;
			case COMMAND_HLT:
				return;
			default:
				colored_fprintf(RED, stderr, "Illegal instruction %i\n", instruction_data[i]);
				return;
		}
		if(err != STATUS_OK) {
			colored_fprintf(RED, stdout, "Error %u occured, exiting\n", err);
			return;
		}
	}
	colored_fprintf(YELLOW, stdout, "Commands ended, but no HLT given\n");
}

static int read_input(const char *path, int **input_data, size_t *input_size) {
	FILE *input = fopen(path, "rb");
	if(input == NULL) {
		fputs("Input file open error\n", stderr);
		return 1;
	}

	long filesize = get_filesize(input);
	if(filesize < 0 || (size_t)filesize%sizeof(int) != 0) {
		fputs("Invalid bytecode file\n", stderr);
		fclose(input);
		return 2;
	}

	*input_data = malloc((size_t)filesize);
	if(*input_data == NULL) {
		fputs("Memory allocation error", stderr);
		fclose(input);
		return 3;
	}

	if(fread(*input_data, sizeof(int), (size_t)filesize/sizeof(int), input) != (size_t)filesize/sizeof(int)) {
		fputs("File read error", stderr);
		fclose(input);
		return 4;
	}

	*input_size = (size_t)filesize/sizeof(int);
	fclose(input);
	return 0;
}

int main(int argc, char **argv) {
	if(argc != 2) {
		puts("program input_file.microcpu");
		return -1;
	}

	int *input_data = NULL;
	size_t input_size = 0;
	int ret = read_input(argv[1], &input_data, &input_size);
	if(ret != 0) return ret;

	StackInt stack = {};
	Status_type err = STATUS_OK;
	stack_int_init(&stack, 1.5, &err);
	if(err != STATUS_OK) printf("Error code: %u\n", err);

	mainloop(&stack, input_data, input_size);

	free(input_data);
	stack_int_destroy(&stack, &err);
	if(err != STATUS_OK) printf("Error code: %u\n", err);
	return 0;
}

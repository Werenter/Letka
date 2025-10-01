#include <stdio.h>
#include <string.h>
#include <math.h>

#include "stack.h"
#include "logger.h"

static void mainloop(StackInt *stack);
static void command_add(StackInt *stack);
static void command_sub(StackInt *stack);
static void command_mul(StackInt *stack);
static void command_div(StackInt *stack);
static void command_sqrt(StackInt *stack);

// Уж лучше макросы, чем копипаста
#define BINARY_OPERATION(stack, operation, name) {\
	if(stack->size < 2) {\
		colored_fprintf(CYAN, stdout, "Stack must have at least two elements for " name " command\n");\
		return;\
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
static void command_add(StackInt *stack) {
	BINARY_OPERATION(stack, +, "ADD");
}

static void command_sub(StackInt *stack) {
	BINARY_OPERATION(stack, -, "SUB");
}

static void command_mul(StackInt *stack) {
	BINARY_OPERATION(stack, *, "MUL");
}

static void command_div(StackInt *stack) {
	BINARY_OPERATION(stack, /, "DIV");
}

static void command_sqrt(StackInt *stack) {
	if(stack->size < 1) {
		colored_fprintf(CYAN, stdout, "Stack must have at least one element for SQRT command\n");
	}
	
	Status_type err1 = STATUS_OK;
	Status_type err2 = STATUS_OK;
	int a = stack_int_pop(stack, &err2);
	stack_int_push((int)sqrt((double)a), stack, &err2);
	if(err1 != STATUS_OK || err2 != STATUS_OK) {
		LOG_ERROR("Internal stack error: %u %u\n", err1, err2);
		STACK_INT_DUMP(stack);
	}
}

void mainloop(StackInt *stack) {
	char command[64];
	Status_type err = STATUS_OK;
	while(true) {
		if(scanf("%63s", command) == EOF) break; // TODO: fgets
		if(strlen(command) > 62) {
			colored_fprintf(RED, stdout, "Incorrect command\n");
		} else if(!strcmp(command, "PUSH")) {
			int number;
			if(scanf("%i \n", &number) != 1) {
				colored_fprintf(RED, stdout, "Incorrect command format\n");
			}
			stack_int_push(number, stack, &err);
			if(err != STATUS_OK) {
				LOG_ERROR("Internal stack error: %u\n", err);
			}
			continue;
		} else if(!strcmp(command, "POP")) {
			int number = stack_int_pop(stack, &err);
			if(err == STATUS_EMPTY) {
				colored_fprintf(CYAN, stdout, "Stack is empty\n");
			} else if(err != STATUS_OK) {
				LOG_ERROR("Internal stack error: %u\n", err);
			} else {
				colored_fprintf(GREEN, stdout, "Removed element: %i\n", number);
			}
		} else if(!strcmp(command, "MUL")) {
			command_mul(stack);
		} else if(!strcmp(command, "ADD")) {
			command_add(stack);
		} else if(!strcmp(command, "SUB")) {
			command_sub(stack);
		} else if(!strcmp(command, "DIV")) {
			command_div(stack);
		} else if(!strcmp(command, "SQRT")) {
			command_sqrt(stack);
		} else if(!strcmp(command, "OUT")) {
			print_stack(stack, stdout, &err);
			if(err != STATUS_OK) LOG_ERROR("Internal stack error: %u\n", err);
		} else if(!strcmp(command, "END")) {
			break;
		} else {
			colored_fprintf(RED, stdout, "Incorrect command\n");
		}
		scanf("\n");
	}
}

int main(void) {
	StackInt stack = {};
	Status_type err = STATUS_OK;
	stack_int_init(&stack, 1.5, &err);
	if(err != STATUS_OK) printf("Error code: %u\n", err);

	mainloop(&stack);

	stack_int_destroy(&stack, &err);
	if(err != STATUS_OK) printf("Error code: %u\n", err);
	return 0;
}

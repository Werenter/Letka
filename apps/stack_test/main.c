#include <stdio.h>

#include "stack.h"

int main(void) {
	StackInt stack = {};
	Status_type err = STATUS_OK;
	stack_int_init(&stack, &err);
	if(err != STATUS_OK) printf("Error code: %u\n", err);

	for(int i = 0; i < 500; i++) {
		stack_int_push(i, &stack, &err);
		if(err != STATUS_OK) printf("Error code: %u\n", err);
	}

	for(int i = 0; i < 500; i++) {
		printf("%i\n", stack_int_pop(&stack, &err));
		if(err != STATUS_OK) printf("Error code: %u\n", err);
	}
	stack_int_destroy(&stack);
}

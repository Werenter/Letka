#include <stdlib.h>
#include <assert.h>

#include "stack.h"

#define BEGIN_STACK_CAPACITY 64

void stack_int_init(StackInt *stack, Status_type *err) {
	assert(stack != NULL);

	*stack = (StackInt){};

	stack->data = calloc(BEGIN_STACK_CAPACITY, sizeof(int));
	if(stack->data == NULL) {
		if(err != NULL) *err = STATUS_ALLOCATION_ERROR;
	} else {
		stack->capacity = BEGIN_STACK_CAPACITY;
		if(err != NULL) *err = STATUS_OK;
	}
}

void stack_int_destroy(StackInt *stack) {
	assert(stack != NULL);

	free(stack->data);
	*stack = (StackInt){};
}

void stack_int_push(int elem, StackInt *stack, Status_type *err) {
	assert(stack != NULL);

	if(stack->size > stack->capacity) {
		if(err != NULL) *err = STATUS_STRUCTURE_ERROR;
		return;
	} else if(stack->size == stack->capacity) {
		int *newarray = realloc(stack->data, sizeof(int)*stack->capacity*2);
		if(newarray == NULL) {
			if(err != NULL) *err = STATUS_ALLOCATION_ERROR;
			return;
		}
		stack->data = newarray;
		stack->capacity *= 2;
	}

	stack->data[stack->size++] = elem;
	if(err != NULL) *err = STATUS_OK;
}

int stack_int_pop(StackInt *stack, Status_type *err) {
	assert(stack != NULL);

	if(stack->size > stack->capacity) {
		if(err != NULL) *err = STATUS_STRUCTURE_ERROR;
		return 0;
	} else if(stack->capacity/4 > stack->size && stack->size > BEGIN_STACK_CAPACITY) {
		int *newarray = realloc(stack->data, sizeof(int)*(stack->capacity/2));
		if(newarray == NULL) {
			if(err != NULL) *err = STATUS_ALLOCATION_ERROR;
			return 0;
		}
		stack->data = newarray;
		stack->capacity /= 2;
	}
	
	if(stack->size == 0) {
		if(err != NULL) *err = STATUS_EMPTY;
		return 0;
	}

	if(err != NULL) *err = STATUS_OK;
	return stack->data[--stack->size];
}

int stack_int_top(StackInt *stack, Status_type *err) {
	assert(stack != NULL);

	if(stack->size > stack->capacity) {
		if(err != NULL) *err = STATUS_STRUCTURE_ERROR;
		return 0;
	}
	
	if(stack->size == 0) {
		if(err != NULL) *err = STATUS_EMPTY;
		return 0;
	}

	if(err != NULL) *err = STATUS_OK;
	return stack->data[stack->size-1];
}

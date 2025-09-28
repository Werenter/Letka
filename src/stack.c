#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "stack.h"
#include "logger.h"

// TODO: Variable canary size
#define BEGIN_STACK_CAPACITY 64
const int canary = 0x20D1FADA;
const int struct_canary = 0x34C1BAAA;

static Status_type integrity_check(StackInt *stack);
static size_t calculate_structure_checksum(StackInt *stack);
static size_t calculate_stack_checksum(StackInt *stack);
static size_t calculate_checksum(const void *data, size_t size);
static Status_type stack_realloc(StackInt *stack, size_t newsize);

static Status_type integrity_check(StackInt *stack) {
	assert(stack != NULL);

	if(stack->begin_canary != struct_canary || stack->end_canary != struct_canary) 
		return STATUS_CANARY_ERROR;

	if(calculate_structure_checksum(stack) != stack->structure_checksum)
		return STATUS_CHECKSUM_ERROR;

	if(stack->size > stack->capacity) 
		return STATUS_STRUCTURE_ERROR;

	if(stack->data[0] != canary || stack->data[stack->capacity+1] != canary)
		return STATUS_CANARY_ERROR;

	if(calculate_stack_checksum(stack) != stack->stack_checksum)
		return STATUS_CHECKSUM_ERROR;

	return STATUS_OK;
}

static size_t calculate_checksum(const void *data, size_t size) {
	size_t chksum = 0;
	for(size_t i = 0; i < size; i++) chksum += ((const uint8_t*)data)[i];
	return chksum;
}

static size_t calculate_structure_checksum(StackInt *stack) {
	size_t old_checksum = stack->structure_checksum;
	stack->structure_checksum = 0;
	size_t checksum = calculate_checksum(stack, sizeof(StackInt));
	stack->structure_checksum = old_checksum;
	return checksum;
}

static size_t calculate_stack_checksum(StackInt *stack) {
	return calculate_checksum(stack->data, (stack->capacity+2)*sizeof(int));
}

void stack_int_init(StackInt *stack, double realoc_coefficient, Status_type *err) {
	assert(stack != NULL);

	*stack = (StackInt){};
	if(realoc_coefficient < 1.2) {
		if(err != NULL) *err = STATUS_COEFFICIENT_ERROR;
		return;
	}

	stack->data = calloc(BEGIN_STACK_CAPACITY+2, sizeof(int));
	if(stack->data == NULL) {
		if(err != NULL) *err = STATUS_ALLOCATION_ERROR;
	} else {
		stack->begin_canary = struct_canary;
		stack->end_canary = struct_canary;
		stack->capacity = BEGIN_STACK_CAPACITY;
		stack->realloc_coefficient = realoc_coefficient;
		stack->data[0] = canary;
		stack->data[stack->capacity+1] = canary;
		stack->stack_checksum = calculate_stack_checksum(stack);
		stack->structure_checksum = calculate_structure_checksum(stack);
		if(err != NULL) *err = STATUS_OK;
	}
}

void stack_int_destroy(StackInt *stack, Status_type *err) {
	assert(stack != NULL);
	Status_type int_check = integrity_check(stack);
	if(err != NULL) *err = int_check;

	if(int_check != STATUS_OK) return;

	free(stack->data);
	*stack = (StackInt){};
}

static Status_type stack_realloc(StackInt *stack, size_t newsize) {
	Status_type ret = STATUS_OK;
	if(newsize >= stack->size) {
		if(newsize > stack->capacity) {
			size_t newcapacity = (size_t)((double)stack->capacity*stack->realloc_coefficient);
			if(newcapacity <= stack->capacity) newcapacity = stack->capacity+1;

			int *newarray = realloc(stack->data, sizeof(int)*(newcapacity+2));
			if(newarray == NULL) {
				ret = STATUS_ALLOCATION_ERROR;
				return ret;
			}
			stack->data = newarray;
			stack->capacity = newcapacity;
			stack->data[stack->capacity+1] = canary;
			stack->stack_checksum = calculate_stack_checksum(stack);
			stack->structure_checksum = calculate_structure_checksum(stack);
		}
	} else {
		size_t border_capacity = (size_t)((double)stack->capacity/stack->realloc_coefficient/stack->realloc_coefficient);
		size_t newcapacity = (size_t)((double)stack->capacity/stack->realloc_coefficient);
		if(border_capacity <= BEGIN_STACK_CAPACITY) return ret;
		if(newcapacity <= BEGIN_STACK_CAPACITY) newcapacity = BEGIN_STACK_CAPACITY;

		if(newcapacity < border_capacity) {
			int *newarray = realloc(stack->data, sizeof(int)*(newcapacity+2));
			if(newarray == NULL) {
				ret = STATUS_ALLOCATION_ERROR;
				return ret;
			}
			stack->data = newarray;
			stack->capacity = newcapacity;
			stack->data[stack->capacity+1] = canary;
			stack->stack_checksum = calculate_stack_checksum(stack);
			stack->structure_checksum = calculate_structure_checksum(stack);
		}
	}
	return ret;
}

// TODO: Custom coefficients
//
// TODO: Write calculator
void stack_int_push(int elem, StackInt *stack, Status_type *err) {
	assert(stack != NULL);
	Status_type err_code = integrity_check(stack);
	if(err_code != STATUS_OK) {
		if(err != NULL) *err = err_code;
		return;
	}

	err_code = stack_realloc(stack, stack->size+1);
	if(err_code != STATUS_OK) {
		if(err != NULL) *err = err_code;
		return;
	}

	stack->data[++stack->size] = elem;
	stack->stack_checksum = calculate_stack_checksum(stack);
	stack->structure_checksum = calculate_structure_checksum(stack);
	if(err != NULL) *err = STATUS_OK;
}

int stack_int_pop(StackInt *stack, Status_type *err) {
	assert(stack != NULL);
	Status_type err_code = integrity_check(stack);
	if(err_code != STATUS_OK) {
		if(err != NULL) *err = err_code;
		return 0;
	}

	if(stack->size == 0) {
		if(err != NULL) *err = STATUS_EMPTY;
		return 0;
	}

	int elem = stack->data[stack->size];
	stack->stack_checksum = calculate_stack_checksum(stack);
	stack->size--;
	stack->structure_checksum = calculate_structure_checksum(stack);

	err_code = stack_realloc(stack, stack->size);
	if(err_code != STATUS_OK) {
		if(err != NULL) *err = err_code;
		return 0;
	}

	if(err != NULL) *err = STATUS_OK;
	return elem;
}

int stack_int_top(StackInt *stack, Status_type *err) {
	assert(stack != NULL);
	Status_type err_code = integrity_check(stack);
	if(err_code != STATUS_OK) {
		if(err != NULL) *err = err_code;
		return 0;
	}
	
	if(stack->size == 0) {
		if(err != NULL) *err = STATUS_EMPTY;
		return 0;
	}

	if(err != NULL) *err = STATUS_OK;
	return stack->data[stack->size];
}

// TODO: Refactor dump
void stack_int_dump(const StackInt *stack) {
	LOG_ERROR("Stack dump:\n");
	LOG_ERROR("Stack size: %lu\n", stack->size);
	LOG_ERROR("Stack capacity: %lu\n", stack->capacity);
	LOG_ERROR("Stack canary: %i\n", canary);
	LOG_ERROR("Stack:\n");
	for(size_t i = 0; i < stack->capacity+2; i++) {
		LOG_ERROR("stack[%lu]: %i\n", i, stack->data[i]);
	}
}

void print_stack(StackInt *stack, FILE *file, Status_type *err) {
	assert(stack != NULL);
	assert(file != NULL);

	Status_type err_code = integrity_check(stack);
	if(err_code != STATUS_OK) {
		if(err != NULL) *err = err_code;
		return;
	}

	if(fputs("Stack data:\n", file) == EOF) {
		if(err != NULL) *err = STATUS_WRITE_ERROR;
		return;
	}
	for(size_t i = 0; i < stack->size; i++) {
		if(fprintf(file, "[%lu]: %i\n", i, stack->data[i+1]) < 0) {
			if(err != NULL) *err = STATUS_WRITE_ERROR;
			return;
		}
	}
	if(err != NULL) *err = STATUS_OK;
}

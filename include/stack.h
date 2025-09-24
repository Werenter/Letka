#include <stdlib.h>

#include "sq_typedefs.h"

/**
 * Structure for saving stack data.
 * Stack of int elements.
 */
typedef struct {
	/**
	 * Stack memory block
	 */
	int *data;
	/**
	 * Stack element count
	 */
	size_t size;
	/**
	 * Allocated space size (must be greater or equal to size)
	 */
	size_t capacity;
} StackInt;


/**
 * Create empty stack.
 *
 * @param [out] stack Pointer to stack structure
 * @param [out] err Optional pointer (can be NULL) to
 * variable for saving status. STATUS_OK if no errors,
 * STATUS_ALLOCATION_ERROR if memory allocation error.
 */
void stack_int_init(StackInt *stack, Status_type *err);

/**
 * Destroy stack.
 *
 * @param [in] stack Stack for destroying.
 */
void stack_int_destroy(StackInt *stack);

/**
 * Push element to stack.
 *
 * Optional err can be STATUS_OK, STATUS_STRUCTURE_ERROR,
 * and STATUS_ALLOCATION_ERROR, if all OK, structure has incorrect
 * parameters and in memory errors cases respectively.
 *
 * @param [in] elem New element added to stack.
 * @param [in,out] stack New element added to stack.
 * @param [out] err Pointer to optional error variable
 */
void stack_int_push(int elem, StackInt *stack, Status_type *err);

/**
 * Pop element from stack.
 *
 * Optional err can be STATUS_OK, STATUS_STRUCTURE_ERROR,
 * and STATUS_ALLOCATION_ERROR, STATUS_EMPTY, if all OK, structure has incorrect
 * parameters, stack is empty, in memory errors cases respectively.
 *
 * @param [in,out] stack Pointer to stack structure.
 * @param [out] err Pointer to optional error variable.
 *
 * @return Element that removed from array.
 */
int stack_int_pop(StackInt *stack, Status_type *err);

/**
 * Get top element in stack.
 *
 * Optional err can be STATUS_OK, STATUS_STRUCTURE_ERROR,
 * and STATUS_EMPTY, if all OK, structure has incorrect
 * parameters and in memory errors cases respectively.
 *
 * @param [in,out] stack Pointer to stack structure
 * @param [out] err Pointer to optional error variable.
 *
 * @return Element that removed from array.
 */
int stack_int_top(StackInt *stack, Status_type *err);

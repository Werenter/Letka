#pragma once

#include <stdlib.h>

typedef struct {
	char *data;
	size_t xsize, ysize;
} square_array;

/*
 * Create square array.
 *
 * This function allocates memory, you must call delete_square_array on returned memory
 * after finishing work with array. Also you must check if pointer in returned struct non-NULL.
 *
 * @param [in] xsize Width of array.
 * @param [in] ysize Height of array.
 *
 * @return Struct with array data
 */
square_array create_square_array(size_t xsize, size_t ysize);

/*
 * Delete square array.
 *
 * This function free memory used by array.
 *
 * @param [in] array Pointer to array for clearing.
 */
void delete_square_array(square_array *array);

/*
 * Get element address from square array.
 *
 * @param [in] x Horizontal position in array.
 * @param [in] y Vertical position in array.
 * @param [in] array Array for work
 */
char *get_address_of_elem_in_square_array(size_t x, size_t y, const square_array array);




typedef struct {
	char *data;
	size_t size;
} triangle_array;

/*
 * Create triangle array.
 *
 * This function allocates memory, you must call delete_triangle_array on returned memory
 * after finishing work with array. Also you must check if pointer in returned struct non-NULL.
 *
 * @param [in] xsize Width and height of triangle array.
 *
 * @return Struct with array data
 */
triangle_array create_triangle_array(size_t size);

/*
 * Delete triangle array.
 *
 * This function free memory used by array.
 *
 * @param [in] array Pointer to array for clearing.
 */
void delete_triangle_array(triangle_array *array);

/*
 * Get element address from triangle array.
 *
 * Invalid pos will return NULL
 *
 * @param [in] x Horizontal position in array.
 * @param [in] y Vertical position in array.
 * @param [in] array Array for work
 */
char *get_address_of_elem_in_triangle_array(size_t x, size_t y, const triangle_array array);

#include <assert.h>
#include <stdio.h>

#include "arrays.h"

square_array create_square_array(size_t xsize, size_t ysize) {
	square_array new_array = {0};

	new_array.data = calloc(xsize*ysize, sizeof(char));

	if(new_array.data != NULL) {
		new_array.xsize = xsize;
		new_array.ysize = ysize;
	}

	return new_array;
}

void delete_square_array(square_array *array) {
	free(array->data);
	array->data = NULL;

	array->xsize = 0;
	array->ysize = 0;
}

char *get_address_of_elem_in_square_array(size_t x, size_t y, const square_array array) {
	assert(x < array.xsize);
	assert(y < array.ysize);
	assert(array.data != NULL);

	return (array.data + y*array.ysize + x);
}

triangle_array create_triangle_array(size_t size) {
	triangle_array new_array = {0};

	new_array.data = calloc((1+size)*size/2, sizeof(char));
	//printf("%lu\n", (1+size)*size/2);

	if(new_array.data != NULL) {
		new_array.size = size;
	}

	return new_array;
}

void delete_triangle_array(triangle_array *array) {
	free(array->data);
	array->data = NULL;

	array->size = 0;
}

char *get_address_of_elem_in_triangle_array(size_t x, size_t y, const triangle_array array) {
	if(x > y) return NULL;
	if(y >= array.size) return NULL;

	size_t offset = (y+1)*(y)/2+x;

	//printf("%lu %lu %lu %c\n", x, y, offset, *(array.data+offset));
	return array.data+offset;
}

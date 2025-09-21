#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <time.h>

#include "strings.h"
#include "sq_io.h"

static Status_type create_index(size_t file_len, string_array *data);
static void slow_qsort_internal(void *base, size_t size, int (*compare)(const void *left, const void *right), size_t begin, size_t end);

static Status_type create_index(size_t file_len, string_array *data) {
	size_t index_count = 0;

	bool is_non_empty = false;
	for(size_t i = 0; i < file_len; i++) {
		if(data->data[i] == '\n' || data->data[i] == '\r') {
			// Drop empty strings
			if(is_non_empty) index_count++;
			is_non_empty = false;
			data->data[i] = '\0';
		} else {
			is_non_empty = true;
		}
	}

	data->index = calloc(index_count, sizeof(char*));
	if(data->index == NULL) {
		return STATUS_ALLOCATION_ERROR;
	}

	is_non_empty = false;
	size_t index_pos = 0;
	for(size_t i = 0; i < file_len; i++) {
		if(data->data[i] != '\0') {
			if(!is_non_empty) {
				is_non_empty = true;
				data->index[index_pos++] = data->data+i;
			}
		} else {
			is_non_empty = false;
		}
	}
	data->index_size = index_count;

	return STATUS_OK;
}

string_array read_string_data(const char *path) {
	string_array data = {};
	size_t file_len = 0;
	Status_type res = read_file(path, &data.data, &file_len);
	if(res != STATUS_OK) {
		return (string_array){};
	}

	res = create_index(file_len, &data);
	if(res != STATUS_OK) {
		free(data.data);
		return (string_array){};
	}

	return data;
}


void clean_string_data(string_array *data) {
	if(data == NULL) return;

	if(data->index != NULL) free(data->index);
	if(data->data  != NULL) free(data->data);
	data->index = NULL;
	data->data  = NULL;
}


int compare_strings_without_special_symbols(const char *str1, const char *str2) {
	while(true) {
		if(*str1 == '\0' && *str2 == '\0') return 0;
		else if(*str1 == '\0') return -1;
		else if(*str2 == '\0') return 1;

		if(!isalpha(*str1)) {
			str1++;
			continue;
		}
		if(!isalpha(*str2)) {
			str2++;
			continue;
		}

		const char cmp1 = (char)tolower(*str1);
		const char cmp2 = (char)tolower(*str2);
		//printf("%c %c       ", cmp1, cmp2);
		if(cmp1 == cmp2) {
			str1++;
			str2++;
		} else if(cmp1 > cmp2) return 1;
		else if(cmp1 < cmp2) return -1;
	}
}

int compare_strings_without_special_symbols_reversed(const char *str1, const char *str2) {
	const size_t len1 = strlen(str1);
	const size_t len2 = strlen(str2);
	if(len1 == 0 && len2 == 0) return 0;
	else if(len1 == 0 && len2 > 0) return -1;
	else if(len1 > 0 && len2 == 0) return 1;
	const char *ptr1 = str1+len1;
	const char *ptr2 = str2+len2;
	while(true) {
		if(ptr1 < str1 && ptr2 < str2) return 0;
		else if(ptr1 < str1) return -1;
		else if(ptr2 < str2) return 1;

		if(!isalpha(*ptr1)) {
			ptr1--;
			continue;
		}
		if(!isalpha(*ptr2)) {
			ptr2--;
			continue;
		}

		const char cmp1 = (char)tolower(*ptr1);
		const char cmp2 = (char)tolower(*ptr2);
		//printf("%c %c       ", cmp1, cmp2);
		if(cmp1 == cmp2) {
			ptr1--;
			ptr2--;
		} else if(cmp1 > cmp2) return 1;
		else if(cmp1 < cmp2) return -1;
	}
}

#define SWAP_MEMORY(type,size,mem1,mem2,idx) {\
	type temp = *((type*)mem1+idx/size);\
	*((type*)mem1+idx/size) = *((type*)mem2+idx/size);\
	*((type*)mem2+idx/size) = temp;\
	idx += size;\
}

void memswp(void *mem1, void *mem2, size_t size) {
	size_t idx = 0;
	size_t swp64 = 0;
	size_t swp32 = 0;
	size_t swp16 = 0;
	size_t swp8 = 0;
	size_t temp_size = size;
	if((size_t)mem1%8 == 0 && (size_t)mem2%8 == 0) {
		swp64 = temp_size/8;
		temp_size -= (temp_size/8)*8;
	}
	if((size_t)mem1%4 == 0 && (size_t)mem2%4 == 0) {
		swp32 = temp_size/4;
		temp_size -= (temp_size/4)*4;
	}
	if((size_t)mem1%2 == 0 && (size_t)mem2%2 == 0) {
		swp16 = temp_size/2;
		temp_size -= (temp_size/2)*2;
	}
	swp8 = temp_size;
	for(size_t i = 0; i < swp64; i++) SWAP_MEMORY(uint64_t, 8, mem1, mem2, idx);
	for(size_t i = 0; i < swp32; i++) SWAP_MEMORY(uint32_t, 4, mem1, mem2, idx);
	for(size_t i = 0; i < swp16; i++) SWAP_MEMORY(uint16_t, 2, mem1, mem2, idx);
	for(size_t i = 0; i < swp8 ; i++) SWAP_MEMORY(uint8_t , 1, mem1, mem2, idx);
}

void slowsort(void *base, size_t n, size_t size, int (*compare)(const void *left, const void *right)) {
	for(size_t i = n-1; i > 0; i--) {
		bool changed_val = false;
		for(size_t j = 0; j < i; j++) {
			//fprintf(stderr, "%lu", j);
			if(compare((uint8_t*)base + size*j, (uint8_t*)base + size*(j+1)) > 0) {
				changed_val = true;
				// TODO: Написать оптимизированный общий swap на uint64_t
				// Можно использовать switch case с проваливанием
				// swap two values
				memswp((uint8_t*)base+size*j, (uint8_t*)base+size*(j+1), size);
			}
		}
		if(changed_val == false) return;
	}
}

static void slow_qsort_internal(void *base, size_t size, int (*compare)(const void *left, const void *right), size_t begin, size_t end) {
	if(end-begin <= 1) return;

	size_t central_index = begin+(size_t)rand()%(end-begin);
	size_t less_count = 0;

	for(size_t i = begin; i < end; i++) {
		if(compare((uint8_t*)base + size*i, (uint8_t*)base + size*central_index) < 0) less_count++;
	}

	if(begin + less_count == end) less_count--;
	memswp((uint8_t*)base + size*central_index, (uint8_t*)base + size*(begin+less_count), size);

	central_index = begin+less_count;

	for(size_t less_index = begin, big_index = central_index+1; less_index < central_index && big_index < end;) {
		if(compare((uint8_t*)base + size*less_index, (uint8_t*)base + size*central_index) > 0) {
			memswp((uint8_t*)base + size*less_index, (uint8_t*)base + size*big_index, size);
			big_index++;
		} else {
			less_index++;
		}
	}

	slow_qsort_internal(base, size, compare, begin, central_index);
	if(central_index != end) slow_qsort_internal(base, size, compare, central_index+1, end);
}

void slow_qsort(void *base, size_t n, size_t size, int (*compare)(const void *left, const void *right)) {
	srand((unsigned int)time(0));
	slow_qsort_internal(base, size, compare, 0, n);
}

void write_string_array(string_array array, FILE *file) {
	for(size_t i = 0; i < array.index_size; i++) {
		fputs(array.index[i], file);
		fputc('\n', file);
	}
}

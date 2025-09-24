#pragma once

#include <stdlib.h>
#include <stdio.h>

/**
 * Typedef struct for indexed text array data
 */
typedef struct {
	/**
	 * Array of pointers to begins of strings in string array
	 */
	char **index;
	/**
	 * Memory block conataining packed array of strings
	 */
	char *data;
	/**
	 * Count of memory blocks in index
	 */
	size_t index_size;
} string_array;

/**
 * Read splitted string data
 *
 * @param [in] path Path to file for reading
 *
 * @return Structure with array. If data pointer is NULL,
 * error occured while reading file.
 */
string_array read_string_data(const char *path);

/**
 * Free string data
 *
 * @param [in] data string_array for clearing
 */
void clean_string_data(string_array *data);

/**
 * This function compares two strings ignoring special symbols and 
 * symbol register. For example, "Abc ds" is equal to "abcds".
 * Behaivour is equalient to strcmp function.
 *
 * @param [in] str1 First string for compare
 * @param [in] str2 Second string for compare
 *
 * @return 0 if str1 and str2 are equal, 1 if str1>str2, -1 if str1<str2
 */
int compare_strings_without_special_symbols(const char *str1, const char *str2);

/**
 * This function compares two strings ignoring special symbols and 
 * symbol register. For example, "Abc ds" is equal to "abcds".
 * Behaivour is equalient to compare_strings_without_special_symbols
 * function, but it compares it not from begin, but from end.
 *
 * @param [in] str1 First string for compare
 * @param [in] str2 Second string for compare
 *
 * @return 0 if str1 and str2 are equal, 1 if str1>str2, -1 if str1<str2
 */
int compare_strings_without_special_symbols_reversed(const char *str1, const char *str2);

/**
 * It is a slow sorting function! qsort has a FATAL FAILURE, 
 * so we made THIS! nlogn is too fast, n^2 is enough!
 *
 * API is equal to qsort.
 *
 * @param [in,out] base Array for sorting
 * @param [in] n Array element count
 * @param [in] size Size of one array element
 * @param [in] compare Comparasion function, if left greater must return 1,
 * if right greater must return -1, if equal must return 0.
 */
void slowsort(void *base, size_t n, size_t size, int (*compare)(const void *left, const void *right));

/**
 * If you too hurried for waiting, while slowsort ends work
 * and don't want to use qsort beware of FATAL FAILURE and NIH,
 * you can use this function!
 *
 * It has nlogn complexity, but is is still slower, than qsort.
 *
 * API is equal to qsort.
 *
 * @param [in,out] base Array for sorting
 * @param [in] n Array element count
 * @param [in] size Size of one array element
 * @param [in] compare Comparasion function, if left greater must return 1,
 * if right greater must return -1, if equal must return 0.
 */
void slow_qsort(void *base, size_t n, size_t size, int (*compare)(const void *left, const void *right));

/**
 * This function saves array of strings to file, strings separated with '\n'.
 *
 * @param [in] string_array Array of writed data
 * @param [in] file File for writing data
 */
void write_string_array(string_array array, FILE *file);

/**
 * Swap two memory regions.
 *
 * size bytes will be swaped between mem1 and mem2 regions.
 *
 * @param [in,out] mem1 First memory region for swap
 * @param [in,out] mem2 Second memory region for swap
 * @param [in] size Count of bytes for swap.
 */
void memswp(void *mem1, void *mem2, size_t size);

/**
 * This function compares two strings ignoring special symbols and 
 * symbol register. For example, "Abc ds" is equal to "abcds".
 * Behaivour is equalient to compare_strings_without_special_symbols_reversed,
 * but checked symbol count limited by complimit param. For example, 
 * "abcdef" and "aaef" are equal with complimit=2.
 *
 * @param [in] str1 First string for compare
 * @param [in] str2 Second string for compare
 * @param [in] complimit Count if chars to compare
 *
 * @return 0 if str1 and str2 are equal, 1 if str1>str2, -1 if str1<str2
 */
int compare_strings_without_special_symbols_reversed_partial(const char *str1, const char *str2, size_t complimit);

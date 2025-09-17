#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "velosiped.h"
#include "arg_parse.h"
#include "sq_io.h"
#include "arrays.h"

const char *filename = NULL;
const char *string_for_search = NULL;
bool will_print_help = false;
bool use_custom_strstr = false;

static Status_type help_argument_callback(const int argc, const char *const *argv, int *current_pos, void *data);
static Status_type filename_callback(const int argc, const char *const *argv, int *current_pos, void *data);
static Status_type string_for_search_callback(const int argc, const char *const *argv, int *current_pos, void *data);
static Status_type strstr_callback(const int argc, const char *const *argv, int *current_pos, void *data);

const command_line_arg cmdline_argument_array[] = {
	{
		.short_name = "-h",
		.long_name = "--help",
		.description = "Печать справки по аргументам командной строки",
		.parser_callback = help_argument_callback,
		.data = &will_print_help
	},
	{
		.short_name = "-f",
		.long_name = "--file",
		.description = "Файл, по которому ведётся поиск",
		.parser_callback = filename_callback,
		.data = &filename
	},
	{
		.short_name = "-n",
		.long_name = "--needle",
		.description = "Текст, который ищем в файле",
		.parser_callback = string_for_search_callback,
		.data = &string_for_search
	},
	{
		.short_name = "-u",
		.long_name = "--use-custom-strstr",
		.description = "Использовать собственную реализацию strstr",
		.parser_callback = strstr_callback,
		.data = &use_custom_strstr
	}
};

int main(int argc, const char *const *argv) {
	triangle_array array = create_triangle_array(26);
	for(size_t y = 0; y < 26; y++) {
		for(size_t x = 0; x <= y; x++) {
			*get_address_of_elem_in_triangle_array(x, y, array) = 'a'+x;
		}
	}

	for(size_t y = 0; y < 26; y++) {
		for(size_t x = 0; x <= y; x++) {
			putchar(*get_address_of_elem_in_triangle_array(x, y, array));
		}
		putchar('\n');
	}
	delete_triangle_array(&array);

	Status_type parse_result = parse_args(argc, argv, ARRAY_LENGTH(cmdline_argument_array), cmdline_argument_array);
	if(parse_result != STATUS_OK || will_print_help || filename == NULL || string_for_search == NULL) {
		puts("Это бенчмарк для функций поиска");
		print_help(ARRAY_LENGTH(cmdline_argument_array), cmdline_argument_array);
		return 0;
	}

	char *file_data;
	Status_type file_read_result = read_file(filename, &file_data);
	if(file_read_result != STATUS_OK) {
		puts("File read error");
		return 1;
	}

	char *pos = NULL;

	if(use_custom_strstr) pos = velo_strstr(file_data, string_for_search);
	else pos = strstr(file_data, string_for_search);

	if(pos != NULL) puts(pos);

	free(file_data);
}

static Status_type help_argument_callback(const int argc, const char *const *argv, int *current_pos, void *data) {
	UNUSED(argc);
	UNUSED(argv);
	UNUSED(current_pos);
	assert(data != NULL);
	*((bool*)data) = true;
	return STATUS_OK;
}

static Status_type filename_callback(const int argc, const char *const *argv, int *current_pos, void *data) {
	if(*current_pos+1 >= argc) return STATUS_ARG_PARSE_ERROR;
	assert(data != NULL);

	*((const char**)data) = argv[++(*current_pos)];
	return STATUS_OK;
}

static Status_type string_for_search_callback(const int argc, const char *const *argv, int *current_pos, void *data) {
	if(*current_pos+1 >= argc) return STATUS_ARG_PARSE_ERROR;
	assert(data != NULL);

	*((const char**)data) = argv[++(*current_pos)];
	return STATUS_OK;
}

static Status_type strstr_callback(const int argc, const char *const *argv, int *current_pos, void *data) {
	UNUSED(argc);
	UNUSED(argv);
	UNUSED(current_pos);
	assert(data != NULL);
	*((bool*)data) = true;
	return STATUS_OK;
}

#include <stdio.h>
#include "strings.h"


int compare_direct(const void *left, const void *right);
int compare_reversed(const void *left, const void *right);
int compare_indexes(const void *left, const void *right);

static void slowsort_string_array(string_array *data, int (*compare)(const void *left, const void *right));

int compare_direct(const void *left, const void *right) {
	return compare_strings_without_special_symbols(*(const char* const*)left, *(const char* const*)right);
}

int compare_reversed(const void *left, const void *right) {
	return compare_strings_without_special_symbols_reversed(*(const char* const*)left, *(const char* const*)right);
}

int compare_indexes(const void *left, const void *right) {
	return *(const char* const*)left < *(const char* const*)right ? -1 : 1;
}

static void slowsort_string_array(string_array *data, int (*compare)(const void *left, const void *right)) {
	slow_qsort(data->index, data->index_size, sizeof(data->index[0]), compare);
}

int main(int argc, const char *const *argv) {
	if(argc != 3) {
		puts("./onegin input_file output_file");
		return -1;
	}

	string_array data = read_string_data(argv[1]);
	if(data.index == NULL || data.data == NULL) {
		fputs("File read error\n", stderr);
		return 1;
	}

	FILE *output_file = fopen(argv[2], "w");
	if(output_file == NULL) {
		fputs("Output file open error\n", stderr);
		clean_string_data(&data);
		return 2;
	}

	fputs("Прямой словарь:" "\n", output_file);
	slowsort_string_array(&data, compare_direct);
	write_string_array(data, output_file);

	fputs("\n\n\n\n" "Обратный словарь:" "\n", output_file);
	slowsort_string_array(&data, compare_reversed);
	write_string_array(data, output_file);

	fputs("\n\n\n\n" "Исходный текст:" "\n", output_file);
	slowsort_string_array(&data, compare_indexes);
	write_string_array(data, output_file);

	fclose(output_file);
	clean_string_data(&data);

	return 0;
}

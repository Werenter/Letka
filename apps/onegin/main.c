#include <stdio.h>
#include <time.h>
#include "strings.h"


int compare_direct(const void *left, const void *right);
int compare_reversed(const void *left, const void *right);
int compare_indexes(const void *left, const void *right);

static void write_bred(string_array *array, FILE *file, int iterations);
static void generate_ryphm_pair(string_array *array, size_t *index1, size_t *index2);
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

static void generate_ryphm_pair(string_array *array, size_t *index1, size_t *index2) {

	size_t begin_index = ((size_t)rand())%array->index_size;
	int direction = rand()%2;

	size_t end_index = begin_index;
	if(direction == 0) {
		for(size_t i = begin_index+1; i < array->index_size; i++) {
			if(compare_strings_without_special_symbols_reversed_partial(array->index[begin_index], array->index[i], 4) != 0) {
				end_index = i-1;
				break;
			}
		}
	} else {
		for(size_t i = begin_index-1; i >= 0; i--) {
			if(compare_strings_without_special_symbols_reversed_partial(array->index[begin_index], array->index[i], 4) != 0) {
				end_index = begin_index;
				begin_index = i+1;
				break;
			}
		}
	}

	*index1 = begin_index;
	//printf("%lu %lu\n", begin_index, end_index);
	*index2 = begin_index + ((size_t)rand())%(end_index+1-begin_index);
}

static void write_bred(string_array *array, FILE *file, int iterations) {
	slowsort_string_array(array, compare_reversed);
	srand((unsigned int)time(0));

	for(int i = 0; i < iterations; i++) {
		size_t index1 = 0;
		size_t index2 = 0;
		size_t index3 = 0;
		size_t index4 = 0;
		generate_ryphm_pair(array, &index1, &index3);
		generate_ryphm_pair(array, &index2, &index4);

		fputs(array->index[index1], file); fputc('\n', file);
		fputs(array->index[index2], file); fputc('\n', file);
		fputs(array->index[index3], file); fputc('\n', file);
		fputs(array->index[index4], file); fputc('\n', file);
		fputc('\n', file);
	}
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

	fputs("\n\n\n\n" "Бред:" "\n", output_file);
	write_bred(&data, output_file, 10);

	fclose(output_file);
	clean_string_data(&data);

	return 0;
}

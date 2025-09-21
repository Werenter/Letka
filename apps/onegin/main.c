#include <stdio.h>
#include "strings.h"


int compare_direct(const void *left, const void *right);
int compare_reversed(const void *left, const void *right);
int compare_indexes(const void *left, const void *right);

static void slowsort_string_array(string_array *data, int (*compare)(const void *left, const void *right));

int compare_direct(const void *left, const void *right) {
	//printf("%p\n", left);
	return compare_strings_without_special_symbols(*(const char* const*)left, *(const char* const*)right);
}

int compare_reversed(const void *left, const void *right) {
	//printf("%p\n", left);
	return compare_strings_without_special_symbols_reversed(*(const char* const*)left, *(const char* const*)right);
}

int compare_indexes(const void *left, const void *right) {
	//printf("%p\n", left);
	return *(const char* const*)left < *(const char* const*)right ? -1 : 1;
}

//int compare_numbers(const void *left, const void *right) {
//	//printf("%p\n", left);
//	return *(const int*)left < *(const int*)right ? -1 : 1;
//}

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

//	int sort_data[] = {0, 7, 3, 5, 4, 6, 9, 8, 1, 2};
//	slow_qsort(sort_data, 10, sizeof(int), compare_numbers);
	//printf("%p\n", data.index);
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

	//char mem1[] = "QWERTYUIOPASDFGHJKLZXCVBNM\0\0\0\0";
	//char mem2[] = "MNBVCXZLKJHGFDSAPOIUYTREWQ\0\0\0\0";
	//memswp(mem1+5, mem2+4, sizeof(mem1)-5);
	//puts(mem1);
	//puts(mem2);
	return 0;
}

#include <stdio.h>

#include "tests.h"

// TODO: аргументы командной строки
// TODO: Makefile с зависимостями от header'ов
int main(int argc, char **argv) {
	if(argc != 3) {
		printf("Use format: ./tests is_zero_test_data square_equation_test_data\n");
		return 1;
	}
	Status_type result = STATUS_BAD_UNKNOWN;
	result = is_zero_test(argv[1]);
	int ret = 0;
	if(result != STATUS_OK) {
		puts("is_zero_test failed");
		ret++;
	}
	result = calc_square_equation_test(argv[2]);
	if(result != STATUS_OK) {
		puts("calc_square_equation_test failed");
		ret++;
	}
	printf("%i tests failed.\n", ret);
	return ret;
}

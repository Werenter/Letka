#include <stdio.h>

#include "tests.h"
#include "arg_parse.h"

// TODO: аргументы командной строки
// TODO: Makefile с зависимостями от header'ов
int main(int argc, const char **argv) {
	int ret = 0;
	Status_type result = STATUS_BAD_UNKNOWN;
	if(argparse_is_flag_set(argc, argv, "--help") || argparse_is_flag_set(argc, argv, "-h")) {
		puts("Это программа для тестирования, поддерживаемые аргументы командной строки:\n"
		     "  --zero-test filename             -  путь к тестовым данным для функции is_zero()\n"
		     "  --square-equation-test filename  -  путь к тестовым данным для квадратного уравнения");
		return 0;
	}
	const char *is_zero_filename = argparse_get_flag_argument(argc, argv, "--zero-test");
	if(is_zero_filename != NULL) {
		result = is_zero_test(is_zero_filename);
		if(result != STATUS_OK) {
			puts("is_zero_test failed");
			ret++;
		}
	}
	const char *calc_square_equation_filename = argparse_get_flag_argument(argc, argv, "--square-equation-test");
	if(calc_square_equation_filename != NULL) {
		result = calc_square_equation_test(calc_square_equation_filename);
		if(result != STATUS_OK) {
			puts("calc_square_equation_test failed");
			ret++;
		}
	}
	printf("%i tests failed.\n", ret);
	return ret;
}

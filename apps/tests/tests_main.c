#include <stdio.h>

#include "tests.h"
#include "arg_parse.h"
#include "cool_assert.h"

// Internal macro for reduse copypaste
#define STDLIB_TEST(NAME)\
	do {\
		result = NAME ();\
		if(result != STATUS_OK) {\
			puts(stringify(NAME) " failed");\
			ret++;\
		}\
	} while(0)

int main(int argc, const char **argv) {
	int ret = 0;
	Status_type result = STATUS_BAD_UNKNOWN;
	if(argparse_is_flag_set(argc, argv, "--help") || argparse_is_flag_set(argc, argv, "-h")) {
		puts("Это программа для тестирования, поддерживаемые аргументы командной строки:\n"
		     "  --zero-test filename             -  путь к тестовым данным для функции is_zero()\n"
		     "  --square-equation-test filename  -  путь к тестовым данным для квадратного уравнения\n"
		     "  --stdlib_tests                   -  запустить тесты для реализаций функций из стандартной библиотеки");
		return 0;
	}
	// --tests zero,square
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
	if(argparse_is_flag_set(argc, argv, "--stdlib_tests")) {
		STDLIB_TEST(velo_strchr_test);
		STDLIB_TEST(velo_strlen_test);

		STDLIB_TEST(velo_strcpy_test);
		STDLIB_TEST(velo_strncpy_test);

		STDLIB_TEST(velo_strcat_test);
		STDLIB_TEST(velo_strncat_test);
	}
	printf("%i tests failed.\n", ret);
	return ret;
}

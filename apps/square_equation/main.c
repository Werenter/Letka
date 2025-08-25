#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "sq_io.h"
#include "square_equation.h"
#include "arg_parse.h"

static void print_help(void);

int main(int argc, const char **argv) {
	if(argparse_is_flag_set(argc, argv, "-h") || argparse_is_flag_set(argc, argv, "--help")) {
		print_help();
		return 0;
	}
	const char **args = NULL;
	if(argparse_is_flag_set(argc, argv, "-c")) {
		args = argparse_get_flag_argument_array(argc, argv, "-c", 3);
		if(args == NULL) {
			print_help();
			return 2;
		}
	} else if(argparse_is_flag_set(argc, argv, "--cli")) {
		args = argparse_get_flag_argument_array(argc, argv, "--cli", 3);
		if(args == NULL) {
			print_help();
			return 2;
		}
	}
	if(args != NULL) {
		coefficients_type coefficients = {.a = NAN, .b = NAN, .c = NAN};
		get_input_coefficients_from_array(args, &coefficients);
		square_equation_result result = calc_square_equation(&coefficients);
		print_square_equation_result(&result);
		return 0;
	}
	puts("Добро пожаловать в программу для решения\n"
	     "квадратных уравнений вида ax²+bx+c=0");
	do {
		coefficients_type coefficients = {.a = NAN, .b = NAN, .c = NAN};
		int res = get_input_coefficients(&coefficients);
		if(res != 0) return 1;
		square_equation_result result = calc_square_equation(&coefficients);
		print_square_equation_result(&result);
	} while(prompt_user_to_continue());
	return 0;
}

static void print_help(void) {
	puts("Это программа для решение квадратных уравнений вида ax²+bx+c=0\n"
	     "Флаги коммандной строки:\n"
	     "  -c, --cli [a b c]  - использовать неинтерактивный режим (коэффициенты указываются как параметры) \n"
	     "  -h, --help  -  печать данной справки");
}

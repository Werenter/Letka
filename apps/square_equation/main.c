#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#include "sq_io.h"
#include "square_equation.h"
#include "arg_parse.h"

static Status_type help_argument_callback(const int argc, const char *const *argv, int *current_pos, void *data);
static Status_type coefficients_argument_callback(const int argc, const char *const *argv, int *current_pos, void *data);

static bool will_print_help = false; // move to main()

static bool cli_coefficients_set = false;
static coefficients_type cli_coefficients = { NAN, NAN, NAN };

const command_line_arg cmdline_argument_array[] = {
	{
		.short_name = "-h",
		.long_name = "--help",
		.description = "Печать справки по аргументам командной строки",
		.parser_callback = help_argument_callback,
		.data = &will_print_help
	},
	{
		.short_name = "-c",
		.long_name = "--cli",
		.description = "CLI-режим работы, коэффициенты указываются в формате {-с, --cli} a b c",
		.parser_callback = coefficients_argument_callback,
		.data = &cli_coefficients
	}
};

int main(int argc, const char **argv) {
	Status_type parse_status = parse_args(argc, argv, ARRAY_LENGTH(cmdline_argument_array), cmdline_argument_array);
	if(will_print_help) {
		puts("Это программа для решения квадратных уравнений вида ax²+bx+c=0");
		puts("Поддерживаемые аргументы командной строки:");
		print_help(ARRAY_LENGTH(cmdline_argument_array), cmdline_argument_array);
		return 0;
	}
	if(parse_status != STATUS_OK) {
		puts("Неверный формат аргументов, используйте следующий:");
		print_help(ARRAY_LENGTH(cmdline_argument_array), cmdline_argument_array);
		return 2;
	}

	if(cli_coefficients_set) {
		square_equation_result result = calc_square_equation(&cli_coefficients);
		print_square_equation_result(&result);
	} else {
		puts("Добро пожаловать в программу для решения\n"
		     "квадратных уравнений вида ax²+bx+c=0");
		do {
			coefficients_type coefficients = {.a = NAN, .b = NAN, .c = NAN};
			int res = get_input_coefficients(&coefficients);
			if(res != 0) return 1;
			square_equation_result result = calc_square_equation(&coefficients);
			print_square_equation_result(&result);
		} while(prompt_user_to_continue());
	}
	puts("\e[5mCOMMIT GITHUB!!!!!!!!!!!!!!!!\e[m");
	return 0;
}

static Status_type help_argument_callback(const int argc, const char *const *argv, int *current_pos, void *data) {
	UNUSED(argc);
	UNUSED(argv);
	UNUSED(current_pos);
	assert(data != NULL);
	*((bool*)data) = true;
	return STATUS_OK;
}

static Status_type coefficients_argument_callback(const int argc, const char *const *argv, int *current_pos, void *data) {
	if(*current_pos+3 >= argc) return STATUS_ARG_PARSE_ERROR;
	assert(data != NULL);
	coefficients_type *coefficients = data;
	bool is_error = 0;
	if(sscanf(argv[++(*current_pos)], "%lf", &coefficients->a) != 1) is_error |= true;
	if(sscanf(argv[++(*current_pos)], "%lf", &coefficients->b) != 1) is_error |= true;
	if(sscanf(argv[++(*current_pos)], "%lf", &coefficients->c) != 1) is_error |= true;
	if(is_error != false) return STATUS_ARG_PARSE_ERROR;
	else {
		cli_coefficients_set = true;
		return STATUS_OK;
	}
}

#pragma once

#include "square_equation.h"

typedef enum {
	FLAG_OK, FLAG_BAD
} Flag_type;

int get_input_number(const char *coef_name, double *var);
void print_square_equation_result(square_equation_result *result);
bool will_cont_work(void);
void clear_stdin(void);
Flag_type clear_stdin_with_check(void);

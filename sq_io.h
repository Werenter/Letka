#pragma once

#include "sq_typedefs.h"

int get_input_number(const char *coef_name, double *var);
int get_input_coefficients(coefficients_type *coefficients);
void print_square_equation_result(square_equation_result *result);
bool will_cont_work(void);
void clear_stdin(void);
Flag_type clear_stdin_with_check(void);

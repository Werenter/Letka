#include <stdio.h>
#include <math.h>

#include "sq_io.h"
#include "square_equation.h"

int main(void) {
	// ax^2 + bx + c = 0
	puts("Добро пожаловать в программу для решения\n"
	     "квадратных уравнений вида ax²+bx+c=0");
	do {
		coefficients_type coefficients = {.a = NAN, .b = NAN, .c = NAN};
		int res = get_input_coefficients(&coefficients);
		if(res != 0) return 1;
		square_equation_result result = calc_square_equation(&coefficients);
		print_square_equation_result(&result);
	} while(will_cont_work());
	return 0;
}

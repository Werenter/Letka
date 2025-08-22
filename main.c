#include <stdio.h>

#include "sq_io.h"
#include "square_equation.h"
#include "cool_assert.h"

int main(void) {
	// ax^2 + bx + c = 0
	puts("Добро пожаловать в программу для решения\n"
	     "квадратных уравнений вида ax²+bx+c=0");
	do {
		int res = 0;
		double a = 0.0;
		double b = 0.0;
		double c = 0.0;
		res = get_input_number("a", &a);
		if(res != 0) return 1;
		res = get_input_number("b", &b);
		if(res != 0) return 1;
		res = get_input_number("c", &c);
		if(res != 0) return 1;
		square_equation_result result = calc_square_equation(a, b, c);
		print_square_equation_result(&result);
	} while(will_cont_work());
	return 0;
}

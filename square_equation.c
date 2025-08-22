#include "square_equation.h"
#include "float.h"

#include <math.h>

square_equation_result calc_linear_equation(const double, const double b, const double c) {
	square_equation_result result = {
		.x1 = 0.0, .x2 = 0.0, .result = NO_ROOTS
	};
	if(is_zero(b)) {
		if(is_zero(c)) {
			result.result = INF_ROOTS;
		} else {
			result.result = NO_ROOTS;
		}
	} else {
		result.result = ONE_ROOT;
		result.x1 = -c/b;
		result.x2 = -c/b;
	}
	return result;
}

square_equation_result calc_only_square_equation(const double a, const double b, const double c) {
	square_equation_result result = {
		.x1 = 0.0, .x2 = 0.0, .result = NO_ROOTS
	};
	double discriminant = b*b-4*a*c;
	if(is_zero(discriminant)) {
		result.result = ONE_ROOT;
		result.x1 = (-b)/2/a;
		result.x2 = (-b)/2/a;
	} else if(discriminant > 0) {
		result.result = TWO_ROOTS;
		double sqrt_discriminant = sqrt(discriminant);
		result.x1 = (-b-sqrt_discriminant)/2/a;
		result.x2 = (-b+sqrt_discriminant)/2/a;
	} else {
		result.result = NO_ROOTS;
	}
	return result;
}

square_equation_result calc_square_equation(const double a, const double b, const double c) {
	square_equation_result result = {
		.x1 = 0.0, .x2 = 0.0, .result = NO_ROOTS
	};
	if(is_zero(a)) {
		result = calc_linear_equation(a, b, c);
	} else {
		result = calc_only_square_equation(a, b, c);
	}
	return result;
}

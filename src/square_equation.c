#include "square_equation.h"
#include "float.h"
#include "cool_assert.h"

#include <math.h>

/**
 * @brief This function solves square equation ax^2+bx+c=0 (a is not equal zero)
 *
 * It gets coefficients from structure coefficients_type
 * and returns square_equation_result structure.
 * 
 * @param [in] coefficients Pointer to struct with equation coefficients.
 * @return  Equation answer
 */
static square_equation_result calc_only_square_equation(const coefficients_type *coefficients);

square_equation_result calc_square_equation(const coefficients_type *coefficients) {
	hard_assert(coefficients != NULL, "Got NULL struct in calc_square_equation");
	if(is_zero(coefficients->a)) {
		return calc_linear_equation(coefficients);
	} else {
		return calc_only_square_equation(coefficients);
	}
}

square_equation_result calc_linear_equation(const coefficients_type *coefficients) {
	hard_assert(coefficients != NULL, "Got NULL struct in calc_linear_equation");
	square_equation_result result = {
		.x1 = NAN, .x2 = NAN, .roots_count = NO_ROOTS
	};
	if(is_zero(coefficients->b)) {
		if(is_zero(coefficients->c)) {
			result.roots_count = INF_ROOTS;
		} else {
			result.roots_count = NO_ROOTS;
		}
	} else {
		result.roots_count = ONE_ROOT;
		result.x1 = -coefficients->c/coefficients->b;
		result.x2 = -coefficients->c/coefficients->b;
	}
	return result;
}

static square_equation_result calc_only_square_equation(const coefficients_type *coefficients) {
	hard_assert(coefficients != NULL, "Got NULL struct in calc_only_square_equation");
	square_equation_result result = {
		.x1 = NAN, .x2 = NAN, .roots_count = NO_ROOTS
	};
	double discriminant = coefficients->b*coefficients->b-4*coefficients->a*coefficients->c;
	if(is_zero(discriminant)) {
		result.roots_count = ONE_ROOT;
		result.x1 = (-coefficients->b)/(2*coefficients->a);
		result.x2 = (-coefficients->b)/(2*coefficients->a);
	} else if(discriminant > 0) {
		result.roots_count = TWO_ROOTS;
		double sqrt_discriminant = sqrt(discriminant);
		result.x1 = (-coefficients->b-sqrt_discriminant)/(2*coefficients->a);
		result.x2 = (-coefficients->b+sqrt_discriminant)/(2*coefficients->a);
	} else {
		result.roots_count = NO_ROOTS;
	}
	return result;
}

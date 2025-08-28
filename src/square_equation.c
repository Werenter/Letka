#include "square_equation.h"
#include "float.h"
#include "cool_assert.h"
#include "logger.h"

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

static Status_type check_solution(const coefficients_type *coefficients, const square_equation_result *result) {
	if(result->roots_count == NO_ROOTS) return STATUS_OK;

	double a = coefficients->a;
	double b = coefficients->b;
	double c = coefficients->c;
	
	double x1 = result->x1;
	double x2 = result->x1;
	if(is_zero(a*x1*x1+b*x1+c) && is_zero(a*x2*x2+b*x2+c)) return STATUS_OK;
	else return STATUS_LOW_PRECISION;
}

square_equation_result calc_square_equation(const coefficients_type *coefficients) {
	hard_assert(coefficients != NULL, "Got NULL struct in calc_square_equation");

	square_equation_result result = {0};
	if(is_zero(coefficients->a)) {
		result = calc_linear_equation(coefficients);
	} else {
		result = calc_only_square_equation(coefficients);
	}
	if(check_solution(coefficients, &result) != STATUS_OK) {
		result.roots_count = CANT_COMPUTE;
		LOG_INFO("To low precision with coefficients (a, b, c): (%lf, %lf, %lf)\n", 
		         coefficients->a, coefficients->b, coefficients->c);
	}
	return result;
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

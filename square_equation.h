#pragma once

typedef struct {
	double x1, x2;
	enum { NO_ROOTS, ONE_ROOT, TWO_ROOTS, INF_ROOTS } result;
} square_equation_result;

square_equation_result calc_square_equation(const double a, const double b, const double c);
square_equation_result calc_linear_equation(const double a, const double b, const double c);
square_equation_result calc_only_square_equation(const double a, const double b, const double c);

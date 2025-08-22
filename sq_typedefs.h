#pragma once

typedef enum {
	FLAG_OK, FLAG_BAD
} Flag_type;

typedef struct {
	double a, b, c;
} coefficients_type;

typedef struct {
	double x1, x2;
	enum { NO_ROOTS, ONE_ROOT, TWO_ROOTS, INF_ROOTS } result;
} square_equation_result;

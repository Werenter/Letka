#pragma once

/**
 * Typedef enum for return values
 */
typedef enum {
	FLAG_OK, ///< All is ok.
	FLAG_BAD ///< All is bad.
} Flag_type;

/**
 * Typedef struct for ax^2+bx+c=0 coefficients
 */
typedef struct {
	/**
	 * @brief Coefficient a for ax^2+bx+c=0 equation
	 */
	double a;
	/**
	 * @brief Coefficient b for ax^2+bx+c=0 equation
	 */
	double b;
	/**
	 * @brief Coefficient c for ax^2+bx+c=0 equation
	 */
	double c;
} coefficients_type;

/**
 * Typedef struct for square equation result
 */
typedef struct {
	/**
	 * First equation root
	 */
	double x1;
	/**
	 * Second equation root
	 */
	double x2;
	/**
	 * Roots count enum.
	 */
	enum { 
		NO_ROOTS,  ///< Equation has no roots.
		ONE_ROOT,  ///< Equation has one root.
		TWO_ROOTS, ///< Equation has two roots.
		INF_ROOTS  ///< Equation has infinity roots.
	} result;
} square_equation_result;

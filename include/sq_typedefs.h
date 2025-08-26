#pragma once

/**
 * Typedef enum for return values
 */
typedef enum {
	STATUS_OK,  ///< All is ok.
	STATUS_BAD_UNKNOWN, ///< All is bad.
	STATUS_EOF, ///< For input functions, used when EOF was found
	STATUS_CHECK_FAILED, ///< Used when checks return bad result
	STATUS_OVERFLOW, ///< For functions with limited buffers, buffer overflow
	STATUS_TEST_FAILED, ///< For tests, test is failed
	STATUS_READ_ERROR,  ///< IO read error
	STATUS_ARG_PARSE_ERROR ///< Command line argument parsing error
} Status_type;

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
	} roots_count;
} square_equation_result;

#define ARRAY_LENGTH(arr) sizeof(arr)/sizeof(arr[0])
#define UNUSED(x) (void)x

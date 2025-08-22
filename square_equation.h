#pragma once

#include "sq_typedefs.h"

/**
 * @brief This function solves general square equation ax^2+bx+c=0
 *
 * It gets coefficients from structure coefficients_type
 * and returns square_equation_result structure.
 * 
 * @param [in] coefficients Pointer to struct with equation coefficients.
 * @return Equation answer
 */
square_equation_result calc_square_equation(const coefficients_type *coefficients);

/**
 * @brief This function solves linear equation bx+c=0 (a not used)
 *
 * It gets coefficients from structure coefficients_type
 * and returns square_equation_result structure.
 * 
 * @param [in] coefficients Pointer to struct with equation coefficients.
 * @return Equation answer
 */
square_equation_result calc_linear_equation(const coefficients_type *coefficients);

/**
 * @brief This function solves square equation ax^2+bx+c=0 (a is not equal zero)
 *
 * It gets coefficients from structure coefficients_type
 * and returns square_equation_result structure.
 * 
 * @param [in] coefficients Pointer to struct with equation coefficients.
 * @return  Equation answer
 */
square_equation_result calc_only_square_equation(const coefficients_type *coefficients);

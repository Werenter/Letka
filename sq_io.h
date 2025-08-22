#pragma once

#include "sq_typedefs.h"

/**
 * @brief This function gets input coefficient from user
 * 
 * @param [in] coef_name Coefficient name for user
 * @param [out] var Pointer to variable for input
 * @return Status, 0 is OK, 1 is BAD
 */
int get_input_number(const char *coef_name, double *var);

/**
 * @brief This function gets all coefficients from user
 * and places it to coefficients_type struct.
 *
 * @param [out] coefficients Pointer to struct for equation coefficients.
 * @return Status 0 is OK, 1 is BAD
 */
int get_input_coefficients(coefficients_type *coefficients);

/**
 * @brief Print equation answer to user.
 * 
 * @param [in] coefficients Pointer to struct with equation result.
 */
void print_square_equation_result(square_equation_result *result);

/**
 * @brief Ask user, continue work or end program.
 *
 * @return Should program end or should continue work
 */
bool prompt_user_to_continue(void);

/**
 * @brief This function clears stdin
 *
 * It drops characters from stdin while '\\n' not found.
 * It can be used for cleaning input when bad input have been given.
 * 
 */
void clear_stdin(void);


/**
 * @brief This function clears stdin
 *
 * It drops characters from stdin while '\\n' not found.
 * It can be used for cleaning input when bad input have been given.
 * Also it checks for non-space symbols on stream found when cleaning.
 * FLAG_OK returned if only space characters found, else FLAG_BAD returned.
 * 
 * @return [Flag_type] Check status
 */
Flag_type clear_stdin_with_check(void);

#pragma once

#include "sq_typedefs.h"

#include <stdio.h>

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
 * @brief This function gets all coefficients from array
 * of strings (size must be at least 3)
 *
 * @param [in] string_array Array of strings with coefficients
 * @param [out] coefficients Pointer to struct for equation coefficients.
 * @return Status 0 is OK, 1 is BAD
 */
int get_input_coefficients_from_array(const char **string_array, coefficients_type *coefficients);

/**
 * @brief Print equation answer to user.
 * 
 * @param [in] coefficients Pointer to struct with equation result.
 */
void print_square_equation_result(const square_equation_result *result);

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
void skip_line(void);


/**
 * @brief This function clears stdin
 *
 * It drops characters from stdin while '\\n' not found.
 * It can be used for cleaning input when bad input have been given.
 * Also it checks for non-space symbols on stream found when cleaning.
 * STATUS_OK returned if only space characters found, else STATUS_CHECK_FAILED returned.
 * 
 * @return [Status_type] Check status
 */
Status_type skip_line_with_check(void);

/**
 * @brief This function gets input line
 *
 * Data from user will be written
 * to buff, but not more, than buffsize.
 *
 * @param [out] buff Buffer for input data
 * @param [in] buffsize Size of buffer
 * 
 * @return [Status_type] STATUS_OVERFLOW returned if buffer to small and
 * data was truncted. In EOF case, STATUS_EOF returned, STATUS_OK returned if all is good
 */
Status_type get_line(char *buff, int buffsize);

/**
 * @brief This function allocate memory block and reads
 * file given by path to it.
 *
 * In case of errors memory will be not allcated (free on buffer
 * will become an error).
 *
 * @param [in] path Path to file (for fopen)
 * @param [out] read_buffer Pointer to pointer for allocated buffer and input data
 * 
 * @return [Status_type] STATUS_OK returned if all work,
 * STATUS_READ_ERROR if any errors occured.
 */
Status_type read_file(const char *path, char **read_buffer);

/**
 * @brief Get size of open file
 *
 * Warning: this function moves lseek to begin
 *
 * @param [in] file FILE* structure with opened file
 * 
 * @return Filesize if OK, -1 if error
 */
long get_filesize(FILE *file);

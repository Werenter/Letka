#pragma once

#include "sq_typedefs.h"

	/**
	 * Pointer to function for processing argument (for example, -r (or --recursive)
	 * flag will call function, that sets bool variable for recursive mode to true
	 *
	 * @param [in] argc Argument Count
	 * @param [in] argv Argument array
	 * @param [in,out] current_pos Pointer to variable means current position in processing argument array,
	 * function can increment it, for example, argument can use additional data from next arg (like `-o program` in gcc).
	 * Function shouldn't increment this variable if it don't use additional parameters (`-o program` for example should
	 * increment this variable only by one).
	 * @param [out] data Pointer to custom data, written by function (for example, pointer to int for number argument)
	 * may be NULL, if parser_callback dont't write custom data.
	 * 
	 * @return STATUS_OK if parsing if all ok, STATUS_ARG_PARSE_ERROR if incorrect input format
	 */
typedef Status_type (*parser_callback_type)(int argc, const char *const *argv, int *current_pos, void *data);

/**
 * Struct for command line arguments
 */
typedef struct {
	/**
	 * Short argument name (like -r, -f, etc)
	 */
	const char *short_name;
	/**
	 * Long argument name (like --recursive, --force, etc)
	 */
	const char *long_name;
	/**
	 * Description for help (like "Remove directories and their contents recursively",
	 * "Ignore nonexistent files and arguments, never prompt", etc)
	 */
	const char *description;
	/**
	 * Pointer to function for processing argument (for example, -r (or --recursive)
	 * flag will call function, that sets bool variable for recursive mode to true
	 */
	parser_callback_type parser_callback;
	/**
	 * Custom data block for parser_callback. Will be given to parser_callback in last argument.
	 */
	void *data;
} command_line_arg;

/**
 * @brief Checks if command line argument exist.
 *
 * @param [in] argc Count of command line arguments
 * @param [in] argv Command line arguments array
 * @param [in] flag Command line flag for check
 *
 * @return true if flag set, false if flag not set
 */
bool argparse_is_flag_set(const int argc, const char **argv, const char *flag);

/**
 * @brief Returns argument after given flag (used first occurrence of flag).
 *
 * With `argparse_get_flag_argument(argc, argv, "-f")` command line `-f test`
 * will return pointer to test.
 *
 * @param [in] argc Count of command line arguments
 * @param [in] argv Command line arguments array
 * @param [in] flag Command line flag for check
 *
 * @return Pointer to found argument if all is ok, NULL if not found
 */
const char *argparse_get_flag_argument(const int argc, const char **argv, const char *flag);

/**
 * @brief Returns argument array after given flag (used first occurrence of flag).
 *
 * Argument array is sequence of argument used for one purporse.
 *
 * With argparse_get_flag_argument_array(argc, argv, "-f", 3) command line `-f test test1 test2`
 * will return pointer to test.
 *
 * @param [in] argc Count of command line arguments
 * @param [in] argv Command line arguments array
 * @param [in] flag Command line flag for check
 * @param [in] arrsize Size of argument array
 *
 * @return Pointer to found argument if all is ok, NULL if not found
 */
const char *const *argparse_get_flag_argument_array(const int argc, const char **argv, const char *flag, const int arrsize);

/**
 * @brief Function for massive command line arguments parsing
 *
 * It gets array of acceptable arguments with callback functions for parsing.
 * These function can do custom data processing, like reading numbers or getting
 * argument arrays.
 *
 * Acceptable arguments in cmdline_arguments_array must correspond to elements in data_array.
 * First argument will use first data_array pointer, second argument will use second data_array pointer, etc.
 *
 * @param [in] argc Count of command line arguments
 * @param [in] argv Command line arguments array
 * @param [in] cmdline_arguments_array_size Count of elements in array of acceptable arguments
 * @param [in,out] cmdline_arguments_array Array of acceptable arguments
 *
 * @return STATUS_OK if all ok, STATUS_ARG_PARSE_ERROR if even one function returned STATUS_ARG_PARSE_ERROR,
 * STATUS_BAD_UNKNOWN if even one function returned another value.
 */
Status_type parse_args(int argc, const char *const *argv, int cmdline_arguments_array_size, const command_line_arg *cmdline_arguments_array);

/*
 * Prints help for command line arguments
 *
 * @param [in] cmdline_arguments_array_size Size of array with cmdline arguments.
 * @param [in] cmdline_arguments_array Array with cmdline arguments
 */
void print_help(const int cmdline_arguments_array_size, const command_line_arg *cmdline_arguments_array);

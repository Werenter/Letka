#pragma once

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
 * This function won't return arguments that begins from '-' symbol.
 *
 * With `argparse_get_flag_argument(argc, argv, "-f")` command line `-f test`
 * will return pointer to test, but with command line `-f -g` result will be NULL.
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
const char **argparse_get_flag_argument_array(const int argc, const char **argv, const char *flag, const int arrsize);

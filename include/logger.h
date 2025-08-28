#pragma once

#include <stdarg.h>
#include <stdio.h>

#include "cool_assert.h"

typedef enum {
	NO_COLOR,
	RED,
	GREEN,
	YELLOW,
	CYAN
} color_type;

/**
 * This function is equalent to vfprintf, but it color output
 *
 * @param [in] file File for writing output.
 * @param [in] color Color for text.
 * @param [in] format Format string for printf.
 * @param [in] ap va_list for vfprintf.
 */
int colored_vfprintf(color_type color, FILE *file, const char *format, va_list va);

/**
 * This function is equalent to fprintf, but it color output.
 *
 * @param [in] file File for writing output.
 * @param [in] color Color for text.
 * @param [in] format Format string for printf.
 * @param [in] VA_ARG Printf arguments.
 */
int colored_fprintf(color_type color, FILE *file, const char *format, ...);

/**
 * This function creates colored log to filelist setted with set_logfile_list.
 * By default used stderr.
 *
 * It is not recommended to use this function directly, it is for internal use
 * in log macros.
 *
 * @param [in] color Color for text.
 * @param [in] file File for writing output.
 * @param [in] log_prefix Prefix written to log.
 * @param [in] format Format string for printf.
 * @param [in] VA_ARG Printf arguments.
 */
void generic_log(color_type color, const char *log_prefix, const char *format, ...);

/**
 * Set list of files for log. Files must be opened and available for write,
 * else behaivour is undefined.
 *
 * @param [in] filelist_size Count of files in list.
 * @param [in] filelist Array of files for logging (not copied, must be
 * available while log used).
 */
void set_logfile_list(size_t filelist_size, FILE *const *filelist);

#ifdef ENABLE_DEBUG

#define LOG_DEBUG(MESSAGE_FORMAT, ...) generic_log(CYAN, "DEBUG:" __FILE__ ":" stringify(__LINE__), MESSAGE_FORMAT __VA_OPT__(,) __VA_ARGS__)

#else

#define LOG_DEBUG(MESSAGE_FORMAT, ...)

#endif


#if (defined ENABLE_DEBUG) || (defined ENABLE_INFO)

#define LOG_INFO(MESSAGE_FORMAT, ...) generic_log(GREEN, "INFO:" __FILE__ ":" stringify(__LINE__), MESSAGE_FORMAT __VA_OPT__(,) __VA_ARGS__)

#else

#define LOG_INFO(MESSAGE_FORMAT, ...)

#endif



#if (defined ENABLE_DEBUG) || (defined ENABLE_INFO) || ((!defined DISABLE_WARNING) && (!defined DISABLE_ERROR))

#define LOG_WARNING(MESSAGE_FORMAT, ...) generic_log(YELLOW, "WARNING:" __FILE__ ":" stringify(__LINE__), MESSAGE_FORMAT __VA_OPT__(,) __VA_ARGS__)

#else

#define LOG_WARNING(MESSAGE_FORMAT, ...)

#endif


#if (defined ENABLE_DEBUG) || (defined ENABLE_INFO) || (!defined DISABLE_ERROR)

#define LOG_ERROR(MESSAGE_FORMAT, ...) generic_log(RED, "ERROR:" __FILE__ ":" stringify(__LINE__), MESSAGE_FORMAT __VA_OPT__(,) __VA_ARGS__)

#else

#define LOG_ERROR(MESSAGE_FORMAT, ...)

#endif

#pragma once

#include <stdarg.h>
#include <stdio.h>

#include "cool_assert.h"
#include "sq_typedefs.h"

typedef enum {
	NO_COLOR,
	RED,
	GREEN,
	YELLOW,
	CYAN
} color_type;

#define LOG_LEVEL_ERROR   1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO    3
#define LOG_LEVEL_DEBUG   4

#ifndef LOG_LEVEL

#define LOG_LEVEL LOG_LEVEL_WARNING

#endif

/**
 * This function is equalent to vfprintf, but it color output
 *
 * For colors used ASCII-sequences (for non-terminal IO colors disabled).
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
 * For colors used ASCII-sequences (for non-terminal IO colors disabled).
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
 * For colors used ASCII-sequences (for non-terminal IO colors disabled).
 *
 * @param [in] color Color for text.
 * @param [in] file File for writing output.
 * @param [in] log_prefix Prefix written to log.
 * @param [in] format Format string for printf.
 * @param [in] VA_ARG Printf arguments.
 */
void generic_log(color_type color, const char *log_prefix, const char *format, ...);

/**
 * Set list of files for plain text logs. Files must be opened and available for write,
 * else behaivour is undefined.
 *
 * @param [in] filelist_size Count of files in list.
 * @param [in] filelist Array of files for logging (not copied, must be
 * available while log used).
 */
void set_logfile_list(size_t filelist_size, FILE *const *filelist);

/**
 * This function enables/disables HTML logs. By default
 * html logs disabled. HTML logs created as files in directory
 * fileprefix. NULL fileprefix means disable of HTML logs.
 * If function was called when non-NULL fileprefix was given before,
 * old log ends and, if fileprefix non-NULL, new log created.
 *
 * @param [in] fileprefix Directory for writing logs.
 */
Status_type set_html_logs(const char *fileprefix);

/**
 * This function must called in the end of log usage if HTML logs have been used, else
 * log will be broken. 
 *
 * This function disables HTML logging and ends created log.
 */
void html_cleanup(void);

#define LOG(COLOR, TYPE, MESSAGE_FORMAT, ...) generic_log(COLOR, TYPE ":" __FILE__ ":" stringify(__LINE__), MESSAGE_FORMAT __VA_OPT__(,) __VA_ARGS__)

#if LOG_LEVEL >= LOG_LEVEL_DEBUG

#define LOG_DEBUG(MESSAGE_FORMAT, ...) generic_log(CYAN, "DEBUG", MESSAGE_FORMAT __VA_OPT__(,) __VA_ARGS__)

#else

#define LOG_DEBUG(MESSAGE_FORMAT, ...)

#endif


#if LOG_LEVEL >= LOG_LEVEL_INFO

#define LOG_INFO(MESSAGE_FORMAT, ...) generic_log(GREEN, "INFO", MESSAGE_FORMAT __VA_OPT__(,) __VA_ARGS__)

#else

#define LOG_INFO(MESSAGE_FORMAT, ...)

#endif



#if LOG_LEVEL >= LOG_LEVEL_WARNING

#define LOG_WARNING(MESSAGE_FORMAT, ...) generic_log(YELLOW, "WARNING", MESSAGE_FORMAT __VA_OPT__(,) __VA_ARGS__)

#else

#define LOG_WARNING(MESSAGE_FORMAT, ...)

#endif


#if LOG_LEVEL >= LOG_LEVEL_ERROR

#define LOG_ERROR(MESSAGE_FORMAT, ...) generic_log(RED, "ERROR", MESSAGE_FORMAT __VA_OPT__(,) __VA_ARGS__)

#else

#define LOG_ERROR(MESSAGE_FORMAT, ...)

#endif

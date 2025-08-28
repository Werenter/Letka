#include "logger.h"
#include "cool_assert.h"

#include <stdarg.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

#include <stdio.h>

#define ANSI_COLOR_RED     "\e[31m"
#define ANSI_COLOR_GREEN   "\e[32m"
#define ANSI_COLOR_YELLOW  "\e[33m"
#define ANSI_COLOR_CYAN    "\e[96m"

#define ANSI_COLOR_RESET   "\e[0m"

static const char *select_color(color_type color);
static void generic_log_internal(color_type color, FILE *file, const char *log_prefix, const char *format, va_list ap);

static FILE *const *filelist_ptr = NULL;
static size_t filelist_size_internal = 0;

static const char *select_color(color_type color) {
	switch(color) {
		case NO_COLOR:
			return ANSI_COLOR_RESET;
		case RED:
			return ANSI_COLOR_RED;
		case GREEN:
			return ANSI_COLOR_GREEN;
		case YELLOW:
			return ANSI_COLOR_YELLOW;
		case CYAN:
			return ANSI_COLOR_CYAN;
		default:
			soft_assert(0, "Incorrect color %u", color);
			return ANSI_COLOR_RESET;
	}
}

int colored_vfprintf(color_type color, FILE *file, const char *format, va_list ap) {
	assert(file != NULL);
	assert(format != NULL);

	
	if(isatty(fileno(file))) fputs(select_color(color), file);
	int ret = vfprintf(file, format, ap);
	if(isatty(fileno(file))) fputs(ANSI_COLOR_RESET, file);
	return ret;
}

int colored_fprintf(color_type color, FILE *file, const char *format, ...) {
	assert(file != NULL);
	assert(format != NULL);

	va_list ap;
	va_start(ap, format);
	return colored_vfprintf(color, file, format, ap);
}

static void generic_log_internal(color_type color, FILE *file, const char *log_prefix, const char *format, va_list ap) {
	assert(file != NULL);
	assert(log_prefix != NULL);
	assert(format != NULL);

	time_t current_time = time(NULL);
	struct tm *timestruct = NULL;
	timestruct = localtime(&current_time);
	assert(timestruct != NULL);

	colored_fprintf(color, file, "[%s]: %.2i.%.2i.%.4i %.2i:%.2i:%.2i ", log_prefix,
	                timestruct->tm_mday, timestruct->tm_mon+1, timestruct->tm_year+1900,
	                timestruct->tm_hour, timestruct->tm_min, timestruct->tm_sec);
	colored_vfprintf(color, file, format, ap);
}

void generic_log(color_type color, const char *log_prefix, const char *format, ...) {
	va_list ap;

	if(filelist_ptr == NULL) {
		va_start(ap, format);
		generic_log_internal(color, stderr, log_prefix, format, ap);
	} else {
		for(size_t i = 0; i < filelist_size_internal; i++) {
			assert(filelist_ptr[i] != NULL);

			va_start(ap, format);
			generic_log_internal(color, filelist_ptr[i], log_prefix, format, ap);
		}
	}
}

void set_logfile_list(size_t filelist_size, FILE *const *filelist) {
	filelist_size_internal = filelist_size;
	filelist_ptr = filelist;
}

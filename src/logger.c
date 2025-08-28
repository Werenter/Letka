#include "logger.h"
#include "cool_assert.h"

#include <stdarg.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define ANSI_COLOR_RED     "\e[31m"
#define ANSI_COLOR_GREEN   "\e[32m"
#define ANSI_COLOR_YELLOW  "\e[33m"
#define ANSI_COLOR_CYAN    "\e[96m"

#define ANSI_COLOR_RESET   "\e[0m"

// length of "/2025_08_28_16_09_24_1234000123_log.html" string + zero character
// 200 символов точно хватит, но является избыточным
#define FILENAME_LENGTH 200

static const char *select_color(color_type color);
static void generic_log_internal(color_type color, FILE *file, const char *log_prefix, const char *format, va_list ap);
static void HTML_log_internal(color_type color, const char *log_prefix, const char *format, va_list ap);

static FILE *const *filelist_ptr = NULL;
static size_t filelist_size_internal = 0;

static FILE *HTML_log = NULL;

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

static const char *select_HTML_color(color_type color) {
	switch(color) {
		case NO_COLOR:
			return "";
		case RED:
			return "red-text";
		case GREEN:
			return "green-text";
		case YELLOW:
			return "yellow-text";
		case CYAN:
			return "cyan-text";
		default:
			soft_assert(0, "Incorrect color %u", color);
			return "";
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

	colored_fprintf(color, file, "%.2i.%.2i.%.4i %.2i:%.2i:%.2i [%s] ",
	                timestruct->tm_mday, timestruct->tm_mon+1, timestruct->tm_year+1900,
	                timestruct->tm_hour, timestruct->tm_min, timestruct->tm_sec,
	                log_prefix);
	colored_vfprintf(color, file, format, ap);
}

void generic_log(color_type color, const char *log_prefix, const char *format, ...) {
	va_list ap;

	va_start(ap, format);
	HTML_log_internal(color, log_prefix, format, ap);

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

Status_type set_html_logs(const char *fileprefix) {
	// Alloc memory for string like "fileprefix/2025_08_28_16_09_24_1234000123_log.html";
	char *filename = calloc(strlen(fileprefix)+FILENAME_LENGTH, sizeof(char));

	strcpy(filename, fileprefix);
	// Add slash if needed
	if(filename[strlen(filename)-1] != '/') strcat(filename, "/");

	char date_filename[FILENAME_LENGTH] = {0};

	time_t current_time = time(NULL);
	struct tm *timestruct = NULL;
	timestruct = localtime(&current_time);

	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);

	snprintf(date_filename, FILENAME_LENGTH, "%.4i_%.2i_%.2i_%.2i_%.2i_%.2i_%.10li_log.html",
	                timestruct->tm_year+1900, timestruct->tm_mon+1, timestruct->tm_mday,
	                timestruct->tm_hour, timestruct->tm_min, timestruct->tm_sec, spec.tv_nsec);
	strcat(filename, date_filename);

	HTML_log = fopen(filename, "w");
	free(filename);
	if(HTML_log == NULL) return STATUS_FILE_OPEN_ERROR;
	else {
		setvbuf(HTML_log, NULL, _IONBF, 0);
		fputs("<!DOCTYPE html>\n"
		      "<html>\n"
		      "<head>\n"
		      "  <meta charset=\"utf-8\">\n"
		      "  <style>\n"
		      "    body {\n"
		      "      font-family: monospace;\n"
		      "    }\n"
		      "    .red-text {\n"
		      "      color: red;\n"
		      "    }\n"
		      "    .yellow-text {\n"
		      "      color: yellow;\n"
		      "    }\n"
		      "    .green-text {\n"
		      "      color: green;\n"
		      "    }\n"
		      "    .cyan-text {\n"
		      "      color: cyan;\n"
		      "    }\n"
		      "  </style>\n"
		      "</head>\n"
		      "<body>\n", 
		      HTML_log);
		return STATUS_OK;
	}
}

void html_cleanup(void) {
	if(HTML_log != NULL) {
		fputs("</body>\n"
		      "</html>\n",
		       HTML_log);
		fclose(HTML_log);
	}
}

static void HTML_log_internal(color_type color, const char *log_prefix, const char *format, va_list ap) {
	if(HTML_log == NULL) return;

	assert(log_prefix != NULL);
	assert(format != NULL);

	time_t current_time = time(NULL);
	struct tm *timestruct = NULL;
	timestruct = localtime(&current_time);
	assert(timestruct != NULL);

	fprintf(HTML_log, "<p class=\"%s\">%.2i.%.2i.%.4i %.2i:%.2i:%.2i [%s] ",
	        select_HTML_color(color),
	        timestruct->tm_mday, timestruct->tm_mon+1, timestruct->tm_year+1900,
	        timestruct->tm_hour, timestruct->tm_min, timestruct->tm_sec,
	        log_prefix);
	vfprintf(HTML_log, format, ap);
	fputs("</p>", HTML_log);
}

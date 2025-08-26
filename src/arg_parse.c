#include "arg_parse.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>

static int get_callback_pos(int cmdline_arguments_array_size, const command_line_arg *cmdline_arguments_array, const char *arg);

static int get_callback_pos(int cmdline_arguments_array_size, const command_line_arg *cmdline_arguments_array, const char *arg) {
	for(int i = 0; i < cmdline_arguments_array_size; i++) { // get_callback

		assert(cmdline_arguments_array[i].parser_callback != NULL);
		assert(cmdline_arguments_array[i].short_name != NULL);
		assert(cmdline_arguments_array[i].long_name != NULL);

		if(!strcmp(cmdline_arguments_array[i].short_name, arg) ||
		   !strcmp(cmdline_arguments_array[i].long_name , arg)) {
			return i;
		}
	}
	return -1;
}

bool argparse_is_flag_set(const int argc, const char **argv, const char *flag) {
	assert(argv != NULL);
	for(int i = 0; i < argc; i++) {
		assert(argv[i] != NULL);
		if(strcmp(argv[i], flag) == 0) return true;
	}
	return false;
}

const char *argparse_get_flag_argument(const int argc, const char **argv, const char *flag) {
	assert(argv != NULL);
	for(int i = 0; i < argc; i++) {
		assert(argv[i] != NULL);
		if(strcmp(argv[i], flag) == 0) {
			if(i+1 >= argc) return NULL;
			else {
				assert(argv[i+1] != NULL);
				return argv[i+1];
			}
		}
	}
	return NULL;
}

const char *const *argparse_get_flag_argument_array(const int argc, const char **argv, const char *flag, const int arrsize) {
	assert(argv != NULL);
	for(int i = 0; i < argc; i++) {
		assert(argv[i] != NULL);
		if(strcmp(argv[i], flag) == 0) {
			if(i+arrsize >= argc) return NULL;
			else return argv+i+1;
		}
	}
	return NULL;
}

/*

#define DEBUG --> включены все логи выше дебага
#define WARNING --> включены только WARNING и ERROR

Либо в файл, либо в stderr (но если перенаправлен, то не красить)

log_init/destruct
LOG([DEBUG|INFO|WARNING|ERROR], "Value = %d", 5)

[14:15.59.132] main.c:15 ERROR  

*/

Status_type parse_args(const int argc, const char *const *argv, const int cmdline_arguments_array_size, const command_line_arg *cmdline_arguments_array) {
	Status_type status = STATUS_OK;

	assert(argv != NULL);
	assert(cmdline_arguments_array != NULL);

	for(int pos = 0; pos < argc; pos++) {
		int callback_pos = get_callback_pos(cmdline_arguments_array_size, cmdline_arguments_array, argv[pos]);
		if(callback_pos < 0) continue;

		Status_type ret = cmdline_arguments_array[callback_pos].parser_callback(argc, argv, &pos, cmdline_arguments_array[callback_pos].data);
		if(ret != STATUS_OK) {
			if(status != STATUS_BAD_UNKNOWN) {
				if(ret == STATUS_ARG_PARSE_ERROR) status = STATUS_ARG_PARSE_ERROR;
				else status = STATUS_BAD_UNKNOWN;
			}
		}
	}

	return status;
}

void print_help(const int cmdline_arguments_array_size, const command_line_arg *cmdline_arguments_array) {
	assert(cmdline_arguments_array != NULL);
	for(int i = 0; i < cmdline_arguments_array_size; i++) {
		printf("  %s, %-40s%s\n", cmdline_arguments_array[i].short_name, cmdline_arguments_array[i].long_name, cmdline_arguments_array[i].description);
	}
}

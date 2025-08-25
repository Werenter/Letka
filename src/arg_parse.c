#include "arg_parse.h"

#include <string.h>
#include <assert.h>

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
				// Проверка, является ли следующий аргумент флагом командной строки
				if(argv[i+1][0] == '-') return NULL;
				else return argv[i+1];
			}
		}
	}
	return NULL;
}

const char **argparse_get_flag_argument_array(const int argc, const char **argv, const char *flag, const int arrsize) {
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

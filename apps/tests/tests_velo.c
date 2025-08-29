#include <string.h>

#include "tests.h"
#include "sq_typedefs.h"
#include "velosiped.h"

typedef struct {
	char c;
	const char *s;
} strchr_test;

strchr_test velo_strchr_test_data[] = {
	{ .c = 'g',  .s = "Treetag" },
	{ .c = '\0', .s = "test" },
	{ .c = 'G',  .s = "BIG BROTHER WATCHING YOU" },
	{ .c = 'X',  .s = "Pupupupu..." },
	{ .c = '\n', .s = "Ban\nBan\nBan\n" },
	{ .c = ' ',  .s = "GGGGGG" },
};

Status_type velo_strchr_test(void) {
	Status_type result = STATUS_OK;
	for(size_t i = 0; i < ARRAY_LENGTH(velo_strchr_test_data); i++) {
		strchr_test test = velo_strchr_test_data[i];
		if(strchr(test.s, test.c) != velo_strchr(test.s, test.c)) {
			printf("strchr test failed with data s = %s and c = %c\n", test.s, test.c);
			result = STATUS_TEST_FAILED;
		}
	}
	return result;
};

//////////////////////////////////////////////////

typedef struct {
	const char *s;
} strlen_test;

strlen_test velo_strlen_test_data[] = {
	{ .s = "Treetag" },
	{ .s = "test" },
	{ .s = "BIG BROTHER WATCHING YOU" },
	{ .s = "Pupupupu..." },
	{ .s = "Ban\nBan\nBan\n" },
	{ .s = "" },
};

Status_type velo_strlen_test(void) {
	Status_type result = STATUS_OK;
	for(size_t i = 0; i < ARRAY_LENGTH(velo_strlen_test_data); i++) {
		strlen_test test = velo_strlen_test_data[i];
		if(strlen(test.s) != velo_strlen(test.s)) {
			printf("strlen test failed with data s = %s\n", test.s);
			result = STATUS_TEST_FAILED;
		}
	}
	return result;
};

//////////////////////////////////////////////////

typedef struct {
	const char *s;
} strcpy_test;

strcpy_test velo_strcpy_test_data[] = {
	{ .s = "Treetag" },
	{ .s = "test" },
	{ .s = "BIG BROTHER WATCHING YOU" },
	{ .s = "Pupupupu..." },
	{ .s = "Ban\nBan\nBan\n" },
	{ .s = "" },
};

Status_type velo_strcpy_test(void) {
	Status_type result = STATUS_OK;
	for(size_t i = 0; i < ARRAY_LENGTH(velo_strcpy_test_data); i++) {
		strcpy_test test = velo_strcpy_test_data[i];
		char test_string[40] = {};
		char  ref_string[40] = {};
		strcpy(ref_string, test.s);
		velo_strcpy(test_string, test.s);
		if(!memcmp(test_string, ref_string, 40)) {
			printf("strcpy test failed with data s = %s\n", test.s);
			result = STATUS_TEST_FAILED;
		}
	}
	return result;
};

Status_type velo_strncpy_test(void) {
	Status_type result = STATUS_OK;
	for(size_t i = 0; i < ARRAY_LENGTH(velo_strcpy_test_data); i++) {
		strcpy_test test = velo_strcpy_test_data[i];
		char test_string[5] = {};
		char  ref_string[5] = {};
		strncpy(ref_string, test.s, 5);
		velo_strncpy(test_string, test.s, 5);
		if(!memcmp(test_string, ref_string, 5)) {
			printf("strncpy test failed with data s = %s\n", test.s);
			result = STATUS_TEST_FAILED;
		}
	}
	return result;
};

//////////////////////////////////////////////////

typedef struct {
	const char *first;
	const char *second;
} strcat_test;

strcat_test velo_strcat_test_data[] = {
	{ .first = "Treetag",                  .second = "tralalelo tralala" },
	{ .first = "test",                     .second = "GOOOOOOOAAAAAL" },
	{ .first = "BIG BROTHER WATCHING YOU", .second = "GOIDA!!!!" },
	{ .first = "Pupupupu...",              .second = "rm -rf /*" },
	{ .first = "Ban\nBan\nBan\n",          .second = "" },
	{ .first = "",                         .second = "" },
};

Status_type velo_strcat_test(void) {
	Status_type result = STATUS_OK;
	for(size_t i = 0; i < ARRAY_LENGTH(velo_strcpy_test_data); i++) {
		strcat_test test = velo_strcat_test_data[i];
		char test_string[40] = {};
		char  ref_string[40] = {};
		strcpy(ref_string, test.first);
		strcat(ref_string, test.second);
		velo_strcpy(test_string, test.first);
		velo_strcat(test_string, test.second);
		if(!memcmp(test_string, ref_string, 40)) {
			printf("strcat test failed with data first = %s, second = %s\n", test.first, test.second);
			result = STATUS_TEST_FAILED;
		}
	}
	return result;
};

Status_type velo_strncat_test(void) {
	Status_type result = STATUS_OK;
	for(size_t i = 0; i < ARRAY_LENGTH(velo_strcpy_test_data); i++) {
		strcat_test test = velo_strcat_test_data[i];
		char test_string[40] = {};
		char  ref_string[40] = {};
		strcpy(ref_string, test.first);
		strncat(ref_string, test.second, 5);
		velo_strcpy(test_string, test.first);
		velo_strncat(test_string, test.second, 5);
		if(!memcmp(test_string, ref_string, 40)) {
			printf("strcat test failed with data first = %s, second = %s\n", test.first, test.second);
			result = STATUS_TEST_FAILED;
		}
	}
	return result;
};

#include "tests.h"

#include "float.h"
#include "square_equation.h"
#include "sq_io.h"
#include "cool_assert.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_LENGTH(arr) sizeof(arr)/sizeof(arr[0])

typedef struct {
	double val;
	int result;
} is_zero_test_data;

static is_zero_test_data read_zero_test_line(const char *line);

Status_type is_zero_test(const char *path) {
	Status_type result = STATUS_OK;
	char *buff = NULL;
	Status_type read_file_res = read_file(path, &buff);
	if(read_file_res != STATUS_OK) {
		puts("Test data file read error");
		return read_file_res;
	}
	char *pos = strtok(buff, "\r\n");
	while(pos != NULL) {
		is_zero_test_data test_data = read_zero_test_line(pos);
		bool res = is_zero(test_data.val);
		if(res != test_data.result) {
			printf("is_zero test failed: with data %.10lf expected %i answer, but got %i\n", 
			       test_data.val, test_data.result, res);
			result = STATUS_TEST_FAILED;
		}
		pos = strtok(NULL, "\r\n");
	}
	free(buff);
	return result;
}

typedef struct {
	coefficients_type input;
	square_equation_result output;
} equation_test_data;

static equation_test_data read_square_test_line(const char *line);

Status_type calc_square_equation_test(const char *path) {
	Status_type result = STATUS_OK;
	char *buff = NULL;
	Status_type read_file_res = read_file(path, &buff);
	if(read_file_res != STATUS_OK) {
		puts("Test data file read error");
		return read_file_res;
	}
	char *pos = strtok(buff, "\n\r");
	while(pos != NULL) {
		equation_test_data test_data = read_square_test_line(pos);
		coefficients_type      input   = test_data.input;
		square_equation_result res     = calc_square_equation(&input);
		square_equation_result ref_res = test_data.output;

		if(!(res.roots_count == ref_res.roots_count && is_equal(res.x1, ref_res.x1)
		                                            && is_equal(res.x2, ref_res.x2))) {

			printf("calc_square_equation_test test failed: with data (a,b,c) (%.10lf,%.10lf,%.10lf) expected (flag,x1,x2) (%u %.10lf %.10lf) answer,"
			       "but got (%u %.10lf %.10lf\n", input.a, input.b, input.c, ref_res.roots_count, ref_res.x1, ref_res.x2, res.roots_count, res.x1, res.x2);

			result = STATUS_TEST_FAILED;
		}
		pos = strtok(NULL, "\n\r");
	}
	free(buff);
	return result;
}

static equation_test_data read_square_test_line(const char *line) {
	hard_assert(line != NULL, "read_square_test_line got NULL");
	equation_test_data test_data = { 
		.input = { .a = 0, .b = 0, .c = 0 }, 
		.output = { .roots_count = NO_ROOTS, .x1 = NAN, .x2 = NAN }
	};

	int scanf_count = sscanf(line, "%lf %lf %lf %u %lf %lf", &test_data.input.a, &test_data.input.b, &test_data.input.c,
	       &test_data.output.roots_count, &test_data.output.x1, &test_data.output.x2);
	if(scanf_count != 6) printf("Incorrect test string: %s", line);
	return test_data;
}

static is_zero_test_data read_zero_test_line(const char *line) {
	hard_assert(line != NULL, "read_zero_test_line got NULL");
	is_zero_test_data test_data = {
		.val = NAN,
		.result = false
	};

	int scanf_count = sscanf(line, "%lf %i", &test_data.val, &test_data.result);
	if(scanf_count != 2) printf("Incorrect test string: %s", line);
	return test_data;
}

#include "sq_io.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "cool_assert.h"
#include <ctype.h>
#include <assert.h>

#define BUFF_SIZE 128

// Очистка ввода 
void skip_line(void) {
	int next = 0;
	do {
		next = getchar();
	} while(next != '\n' && next != EOF);
}

// Очистка ввода с проверкой наличия лишних символов
Status_type skip_line_with_check(void) {
	int next = 0;
	Status_type flag = STATUS_OK;
	do {
		next = getchar();
		if(!isspace(next)) flag = STATUS_CHECK_FAILED;
	} while(next != '\n' && next != EOF);
	return flag;
}

Status_type get_line(char *buff, int buffsize) {
	char *result = fgets(buff, buffsize, stdin);
	if(result == NULL) {
		fputs("Input read error\n", stderr);
		return STATUS_EOF;
	} else {
		// Проверяем, остались ли в потоке символы
		char *pos = strchr(buff, '\n');
		if(pos == NULL) {
			skip_line();
			return STATUS_OVERFLOW;
		} else {
			return STATUS_OK;
		}
	}
}

int get_input_number(const char *prompt, double *var) {
	hard_assert(prompt != NULL, "get_input_number got NULL pointer");
	hard_assert(var    != NULL, "get_input_number got NULL pointer");
	*var = NAN; 

	while(true) {
		double number = NAN;
		printf("Введите коэффициент %s: ", prompt);
		char buff[BUFF_SIZE] = {0};
		Status_type result = get_line(buff, BUFF_SIZE);
		if(result == STATUS_OVERFLOW) {
			puts("To many symbols");
			continue;
		} else if(result == STATUS_EOF) {
			puts("Got EOF on input");
			return 1;
		}

		int character_count = 0;
		int scanf_count = sscanf(buff, "%lf%n", &number, &character_count);
		bool is_valid_string = true;
		if(scanf_count == 1) { // TODO: в функцию
			for(int i = character_count; i < BUFF_SIZE; i++) {
				if(!isspace(buff[i]) && buff[i] != '\0') {
					is_valid_string = false;
				}
			}
		} else {
			is_valid_string = false;
		}
		if(is_valid_string) {
			*var = number;
			return 0;
		} else {
			printf("Incorrect input format, use x.y notation for numbers\n");
			*var = NAN;
		}
	}
}

int get_input_coefficients_from_array(const char **string_array, coefficients_type *coefficients) {
	assert(string_array != NULL);
	assert(coefficients != NULL);
	assert(string_array[0] != NULL);
	if(sscanf(string_array[0], "%lf", &coefficients->a) != 1) return 1;
	assert(string_array[0] != NULL);
	if(sscanf(string_array[1], "%lf", &coefficients->b) != 1) return 1;
	assert(string_array[0] != NULL);
	if(sscanf(string_array[2], "%lf", &coefficients->c) != 1) return 1;
	return 0;
}

// TODO: remove copypaste bez define'ov
// tol'ko ostav' etu function (aos)
int get_input_coefficients(coefficients_type *coefficients) {
	hard_assert(coefficients != NULL, "Got NULL struct in get_input_coefficients");
	int res = 0;
	res |= get_input_number("a", &coefficients->a);
	res |= get_input_number("b", &coefficients->b);
	res |= get_input_number("c", &coefficients->c);
	if(res != 0) return 1;
	return 0;
}

void print_square_equation_result(const square_equation_result *result) {
	hard_assert(result != NULL, "print_square_equation_result got NULL pointer");
	switch(result->roots_count) {
		case TWO_ROOTS:
			printf("Корни уравнения: %.10lf %.10lf\n", result->x1, result->x2);
			break;
		case ONE_ROOT:
			printf("Корень уравнения: %.10lf\n", result->x1);
			break;
		case NO_ROOTS:
			puts("Уравнение не имеет действительных корней");
			break;
		case INF_ROOTS:
			puts("Корнем уравнения является любое действительное число");
			break;
		default:
			soft_assert(0, "Incorrect enum value in print_square_equation_result");
	}
}

bool prompt_user_to_continue(void) {
	char buff[BUFF_SIZE];
	printf("Наберите \"yes\", если хотите продолжить, любой другой ответ завершит программу: ");
	Status_type result = get_line(buff, BUFF_SIZE);
	if(result != STATUS_OK) return false;

	char *pos = strstr(buff, "yes");
	if(pos == NULL) return false;

	for(int i = 0; i < 3; i++) pos[i] = ' ';

	for(int i = 0; i < BUFF_SIZE; i++) {
		if(buff[i] == '\0') break;
		if(!isspace(buff[i])) return false;
	}
	return true;
}

Status_type read_file(const char *path, char **read_buffer) {
	hard_assert(path != NULL, "read_file has NULL pointer");
	hard_assert(read_buffer != NULL, "read_file has NULL pointer");

	FILE *file = fopen(path, "r");
	if(file == NULL) return STATUS_READ_ERROR;

	long filesize = get_filesize(file);
	if(filesize < 0) {
		fclose(file);
		return STATUS_READ_ERROR;
	}

	char *memory_block = calloc((unsigned long)filesize+1, sizeof(char));
	if(memory_block == NULL) {
		fclose(file);
		return STATUS_READ_ERROR;
	}

	fread(memory_block, sizeof(char), (unsigned long)filesize, file);
	if(ferror(file)) {
		fclose(file);
		free(memory_block);
		return STATUS_READ_ERROR;
	}

	fclose(file);

	*read_buffer = memory_block;
	return STATUS_OK;
}

long get_filesize(FILE *file) {
	int status = fseek(file, 0, SEEK_END);
	if(status < 0) return -1;

	long filesize = ftell(file);
	if(filesize < 0) return -1;

	status = fseek(file, 0, SEEK_SET);
	if(status < 0) return -1;
	return filesize;
}

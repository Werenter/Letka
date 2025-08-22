#include "sq_io.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "cool_assert.h"
#include <ctype.h>
#include <assert.h>

// Очистка ввода 
void clear_stdin(void) {
	while(getchar() != '\n') {}
}

// Очистка ввода с проверкой наличия лишних символов
Flag_type clear_stdin_with_check(void) {
	int next = 0;
	Flag_type flag = FLAG_OK;
	do {
		next = getchar();
		if(!isspace(next)) flag = FLAG_BAD;
	} while(next != '\n');
	return flag;
}

int get_input_number(const char *prompt, double *var) {
	hard_assert(prompt != NULL, "get_input_number got NULL pointer");
	hard_assert(var != NULL, "get_input_number got NULL pointer");
	double input = NAN;
	while(true) {
		printf("Введите коэффициент %s: ", prompt);
		int scanf_count = scanf("%lf", &input);
		if(scanf_count == EOF) {
			printf("Got EOF while reading input\n");
			*var = 0.0;
			return 1;
		} else if(scanf_count != 1) {
			printf("Incorrect input format, use x.y notation for numbers\n");
			clear_stdin();
		} else {
			Flag_type flag = clear_stdin_with_check();
			if(flag == FLAG_BAD) {
				printf("Incorrect input format, use x.y notation for numbers\n");
			} else {
				*var = input;
				return 0;
			}
		}
	}
}

int get_input_coefficients(coefficients_type *coefficients) {
	hard_assert(coefficients != NULL, "Got NULL struct in get_input_coefficients");
	int res = 0;
	res = get_input_number("a", &coefficients->a);
	if(res != 0) return 1;
	res = get_input_number("b", &coefficients->b);
	if(res != 0) return 1;
	res = get_input_number("c", &coefficients->c);
	if(res != 0) return 1;
	return 0;
}

void print_square_equation_result(square_equation_result *result) {
	hard_assert(result != NULL, "print_square_equation_result got NULL pointer");
	switch(result->result) {
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

#define BUFF_SIZE 128

bool will_cont_work(void) {
	char buff[BUFF_SIZE] = {0};
	printf("Наберите \"yes\", если хотите продолжить, любой другой ответ завершит программу: ");
	char *result = fgets(buff, BUFF_SIZE, stdin);
	if(result == NULL) {
		puts("Input read error");
		return false;
	}
	if(strcmp(buff, "yes\n") == 0) return true;
	else {
		// Проверяем, остались ли в потоке символы
		char *pos = strchr(buff, '\n');
		if(pos == NULL) {
			clear_stdin();
		}
		return false;
	}
}


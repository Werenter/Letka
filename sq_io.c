#include "sq_io.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

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
		if(next != '\n' && next != ' ' && next != '\t') flag = FLAG_BAD; // Есть станадртная функция
	} while(next != '\n');
	return flag;
}

int get_input_number(const char *prompt, double *var) {
	// assert(...)
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

void print_square_equation_result(square_equation_result *result) {
	if(result->result == TWO_ROOTS) {
		printf("Корни уравнения: %.10lf %.10lf\n", result->x1, result->x2);
	} else if(result->result == ONE_ROOT) {
		printf("Корень уравнения: %.10lf\n", result->x1);
	} else if(result->result == NO_ROOTS) {
		puts("Уравнение не имеет действительных корней");
	} else if(result->result == INF_ROOTS) {
		puts("Корнем уравнения является любое действительное число");
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


#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct {
	double x1, x2;
	enum { NO_ROOTS, ONE_ROOT, TWO_ROOTS, INF_ROOTS } result;
} square_equation_result;

typedef enum {
	FLAG_OK, FLAG_BAD
} Flag_type;

square_equation_result calc_square_equation(const double a, const double b, const double c);
int get_input_number(const char *coef_name, double *var);
void print_square_equation_result(square_equation_result *result);
bool is_zero(double val);
bool will_cont_work(void);
void clear_stdin(void);
Flag_type clear_stdin_with_check(void);
square_equation_result calc_linear_equation(const double a, const double b, const double c);
square_equation_result calc_only_square_equation(const double a, const double b, const double c);

int main(void) {
	// ax^2 + bx + c = 0
	puts("Добро пожаловать в программу для решения\n"
	     "квадратных уравнений вида ax²+bx+c=0");
	do {
		int res = 0;
		double a = 0.0;
		double b = 0.0;
		double c = 0.0;
		res = get_input_number("a", &a);
		if(res != 0) return 1;
		res = get_input_number("b", &b);
		if(res != 0) return 1;
		res = get_input_number("c", &c);
		if(res != 0) return 1;
		square_equation_result result = calc_square_equation(a, b, c);
		print_square_equation_result(&result);
	} while(will_cont_work());
	return 0;
}

const double EPSILON = 0.0000001;

bool is_zero(double val) {
	if(fabs(val) < EPSILON) return true;
	else return false;
}

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

square_equation_result calc_linear_equation(const double, const double b, const double c) {
	square_equation_result result = {
		.x1 = 0.0, .x2 = 0.0, .result = NO_ROOTS
	};
	if(is_zero(b)) {
		if(is_zero(c)) {
			result.result = INF_ROOTS;
		} else {
			result.result = NO_ROOTS;
		}
	} else {
		result.result = ONE_ROOT;
		result.x1 = -c/b;
		result.x2 = -c/b;
	}
	return result;
}

square_equation_result calc_only_square_equation(const double a, const double b, const double c) {
	square_equation_result result = {
		.x1 = 0.0, .x2 = 0.0, .result = NO_ROOTS
	};
	double discriminant = b*b-4*a*c;
	if(is_zero(discriminant)) {
		result.result = ONE_ROOT;
		result.x1 = (-b)/2/a;
		result.x2 = (-b)/2/a;
	} else if(discriminant > 0) {
		result.result = TWO_ROOTS;
		double sqrt_discriminant = sqrt(discriminant);
		result.x1 = (-b-sqrt_discriminant)/2/a;
		result.x2 = (-b+sqrt_discriminant)/2/a;
	} else {
		result.result = NO_ROOTS;
	}
	return result;
}

square_equation_result calc_square_equation(const double a, const double b, const double c) {
	square_equation_result result = {
		.x1 = 0.0, .x2 = 0.0, .result = NO_ROOTS
	};
	if(is_zero(a))
		return calc_linear_equation(a, b, c);

	return calc_only_square_equation(a, b, c);
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


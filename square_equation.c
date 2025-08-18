#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct square_equation_result {
	double x1, x2;
	enum { NO_ROOTS, ONE_ROOT, TWO_ROOTS, INCORRECT_EQUATION } result;
};

struct square_equation_result calc_square_equation(double a, double b, double c);
double get_input_number(char *prompt);
void print_square_equation_result(struct square_equation_result result);

double get_input_number(char *prompt) {
	double input;
	printf("%s: ", prompt);
	int state = scanf("%lf", &input);
	if(state == EOF) {
		printf("Got EOF while reading input, exiting\n");
		exit(1);
	} else if(state != 1) {
		printf("Incorrect input format, use x.y notation for numbers, exiting\n");
		exit(2);
	} else {
		return input;
	}
}

struct square_equation_result calc_square_equation(double a, double b, double c) {
	struct square_equation_result result;
	double discriminant = b*b-4*a*c;
	if(a == 0.0) {
		result.result = INCORRECT_EQUATION;
		return result;
	}
	if(discriminant > 0) {
		result.result = TWO_ROOTS;
		result.x1 = (-b-sqrt(discriminant))/2/a;
		result.x2 = (-b+sqrt(discriminant))/2/a;
	} else if(discriminant == 0.0) {
		result.result = ONE_ROOT;
		result.x1 = (-b)/2/a;
		result.x2 = (-b)/2/a;
	} else {
		result.result = NO_ROOTS;
	}
	return result;
}

void print_square_equation_result(struct square_equation_result result) {
	if(result.result == INCORRECT_EQUATION) {
		puts("Уравнение не является квадратным");
	} else if(result.result == TWO_ROOTS) {
		printf("Корни уравнения: %.10lf %.10lf\n", result.x1, result.x2);
	} else if(result.result == ONE_ROOT) {
		printf("Корень уравнения: %.10lf\n", result.x1);
	} else if(result.result == NO_ROOTS) {
		puts("Уравнение не имеет действительных корней");
	}
}

int main(void) {
	// ax^2 + bx + c = 0
	puts("Добро пожаловать в программу для решения\n"
	     "квадратных уравнений вида ax²+bx+c=0");
	double a = get_input_number("Введите коэффициент a");
	double b = get_input_number("Введите коэффициент b");
	double c = get_input_number("Введите коэффициент c");
	struct square_equation_result result = calc_square_equation(a, b, c);
	print_square_equation_result(result);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double get_input_number(char *prompt);

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

int main(void) {
	// ax^2 + bx + c = 0
	puts("Добро пожаловать в программу для решения\n"
	     "квадратных уравнений вида ax²+bx+c=0");
	double a = get_input_number("Введите коэффициент a");
	double b = get_input_number("Введите коэффициент b");
	double c = get_input_number("Введите коэффициент c");
	if(a == 0.0) {
		puts("Уравнение не является квадратным");
		return 0;
	}
	double discriminant = b*b-4*a*c;
	if(discriminant > 0) {
		double x1 = (-b-sqrt(discriminant))/2/a;
		double x2 = (-b+sqrt(discriminant))/2/a;
		printf("Корни уравнения: %.10lf %.10lf\n", x1, x2);
	} else if(discriminant == 0.0) {
		double x = (-b)/2/a;
		printf("Корень уравнения: %.10lf\n", x);
	} else {
		puts("Уравнение не имеет действительных корней");
	}
	return 0;
}

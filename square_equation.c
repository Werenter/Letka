#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct square_equation_result {
	double x1, x2;
	enum { NO_ROOTS, ONE_ROOT, TWO_ROOTS, INF_ROOTS } result;
};

struct square_equation_result calc_square_equation(double a, double b, double c);
double get_input_number(char *prompt);
void print_square_equation_result(struct square_equation_result result);
bool is_zero(double val);
bool will_cont_work(void);

bool is_zero(double val) {
	if(fabs(val) < 0.0000001) return true;
	else return false;
}

double get_input_number(char *prompt) {
	double input;
	while(true) {
		printf("%s: ", prompt);
		int state = scanf("%lf", &input);
		if(state == EOF) {
			printf("Got EOF while reading input, exiting\n");
			exit(1);
		} else if(state != 1) {
			printf("Incorrect input format, use x.y notation for numbers\n");
			while(getchar() != '\n') {}
		} else {
			while(getchar() != '\n') {}
			return input;
		}
	}
}

struct square_equation_result calc_square_equation(double a, double b, double c) {
	struct square_equation_result result;
	double discriminant = b*b-4*a*c;
	if(is_zero(a)) {
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
	} else {
		if(discriminant > 0) {
			result.result = TWO_ROOTS;
			result.x1 = (-b-sqrt(discriminant))/2/a;
			result.x2 = (-b+sqrt(discriminant))/2/a;
		} else if(is_zero(discriminant)) {
			result.result = ONE_ROOT;
			result.x1 = (-b)/2/a;
			result.x2 = (-b)/2/a;
		} else {
			result.result = NO_ROOTS;
		}
	}
	return result;
}

void print_square_equation_result(struct square_equation_result result) {
	if(result.result == TWO_ROOTS) {
		printf("Корни уравнения: %.10lf %.10lf\n", result.x1, result.x2);
	} else if(result.result == ONE_ROOT) {
		printf("Корень уравнения: %.10lf\n", result.x1);
	} else if(result.result == NO_ROOTS) {
		puts("Уравнение не имеет действительных корней");
	} else if(result.result == INF_ROOTS) {
		puts("Корнем уравнения является любое действительное число");
	}
}

bool will_cont_work(void) {
	char buff[8] = {0};
	printf("Наберите \"yes\", если хотите продолжить, любой другой ответ завершит программу: ");
	int next = getchar();
	int pos = 0;
	bool overflow = false;
	while(next != EOF && next != '\n') {
		if(pos < 7) buff[pos++] = (char)next;
		else overflow = true;
		next = getchar();
	}
	buff[7] = '\0';
	if(!overflow && strcmp(buff, "yes") == 0) return true;
	else return false;
}

int main(void) {
	// ax^2 + bx + c = 0
	puts("Добро пожаловать в программу для решения\n"
	     "квадратных уравнений вида ax²+bx+c=0");
	do {
		double a = get_input_number("Введите коэффициент a");
		double b = get_input_number("Введите коэффициент b");
		double c = get_input_number("Введите коэффициент c");
		struct square_equation_result result = calc_square_equation(a, b, c);
		print_square_equation_result(result);
	} while(will_cont_work());
	return 0;
}

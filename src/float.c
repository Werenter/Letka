#include <math.h>

#include "float.h"

const double EPSILON = 0.0000001;

bool is_zero(double val) {
	if(fabs(val) < EPSILON) return true;
	else return false;
}

bool is_equal(double a, double b) {
	if((isnan(a) && isnan(b)) || fabs(a-b) < EPSILON) return true;
	else return false;
}

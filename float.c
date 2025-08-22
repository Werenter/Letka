#include <math.h>

#include "float.h"

const double EPSILON = 0.0000001;

bool is_zero(double val) {
	if(fabs(val) < EPSILON) return true;
	else return false;
}

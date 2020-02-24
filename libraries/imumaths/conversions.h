#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
static double toDegrees(double a) {
	return a * (180 / M_PI);
}

static double toRadians(double angle) {
	return angle* (M_PI / 180.0);
}
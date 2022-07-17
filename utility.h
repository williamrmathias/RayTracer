#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

#pragma once

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.14159265358;

// Functions

double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

double random_double() {
	//return random number in [0, 1)
	return rand() / (RAND_MAX + 1.0);
}

double random_double(double min, double max) {
	//returns random number in [min, max)
	return min + (max - min) * random_double();
}

double clamp(double x, double min, double max) {
	//restricts a number x to the range [min, max]
	if (x < min) { return min; }
	if (x > max) { return max; }
	return x;
}
#include <cmath>
#include <limits>
#include <memory>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.14159265358;

// Functions

double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}
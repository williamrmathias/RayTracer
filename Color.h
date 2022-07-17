#include <iostream>
#include "Tuple3d.h"

#pragma once

// translate [0, 1.0] pixels values to [0, 255]
// then prints out the pixel values
void print_color(std::ostream& out, color pixel, int num_samples) {
	double r = pixel.x();
	double g = pixel.y();
	double b = pixel.z();

	// Average colors across samples and add gamma correction
	// gamma = 2.0
	double scaling = 1.0 / num_samples;
	r = sqrt(scaling * r);
	g = sqrt(scaling * g);
	b = sqrt(scaling * b);

	out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}
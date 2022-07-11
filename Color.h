#include <iostream>
#include "Tuple3d.h"

#pragma once

// translate [0, 1.0] pixels values to [0, 255]
// then prints out the pixel values
void print_color(std::ostream& out, color pixel, int num_samples) {
	double r = pixel.x();
	double g = pixel.y();
	double b = pixel.z();

	double scaling = 1.0 / num_samples;
	r *= scaling;
	g *= scaling;
	b *= scaling;

	out << static_cast<int>(255.999 * clamp(r, 0.0, 1.0)) << ' '
		<< static_cast<int>(255.999 * clamp(g, 0.0, 1.0)) << ' '
		<< static_cast<int>(255.999 * clamp(b, 0.0, 1.0)) << '\n';
}
#include <iostream>
#include "Tuple3d.h"

#pragma once

// translate [0, 1.0] pixels values to [0, 255]
// then prints out the pixel values
void print_color(std::ostream& out, color pixel) {
	out << static_cast<int>(255.999 * pixel.x()) << ' '
		<< static_cast<int>(255.999 * pixel.y()) << ' '
		<< static_cast<int>(255.999 * pixel.z()) << '\n';
}
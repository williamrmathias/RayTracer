#include "utility.h"

#pragma once

class Camera {
public:
	//default ctor
	Camera() {
		double aspect_ratio = 16.0 / 9.0;
		double viewport_height = 2.0;
		double viewport_width = aspect_ratio * viewport_height;
		double focal_length = 1.0;

		origin = point3d(0, 0, 0);
		horizontal = Tuple3d(viewport_width, 0.0, 0.0);
		vertical = Tuple3d(0.0, viewport_height, 0.0);
		lower_left_corner = origin - (horizontal / 2) - (vertical / 2)
			- Tuple3d(0, 0, focal_length);
	}

	Ray get_ray(double u, double v) const {
		return Ray(origin, lower_left_corner + (u * horizontal) 
			+ (v * vertical) - origin);
	}

private:
	point3d origin;
	point3d lower_left_corner;
	Tuple3d horizontal;
	Tuple3d vertical;
};
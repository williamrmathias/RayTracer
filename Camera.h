#include "utility.h"

#pragma once

class Camera {
public:
	//default ctor
	Camera(
		double vertical_fov,
		double aspect_ratio,
		point3d look_from,
		point3d look_at,
		Tuple3d vup
	) {
		double theta = degrees_to_radians(vertical_fov);
		double height = tan(theta / 2);
		double viewport_height = 2.0 * height;
		double viewport_width = aspect_ratio * viewport_height;

		Tuple3d w = normalize(look_from - look_at);
		Tuple3d u = normalize(cross(vup, w));
		Tuple3d v = cross(w, u);

		origin = look_from;
		horizontal = viewport_width * u;
		vertical = viewport_height * v;
		lower_left_corner = origin - (horizontal / 2) - (vertical / 2) - w;
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
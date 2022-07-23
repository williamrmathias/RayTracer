#include "utility.h"

#pragma once

class Camera {
public:
	//default ctor
	Camera(
		point3d look_from,
		point3d look_at,
		Tuple3d vup,
		double vertical_fov,
		double aspect_ratio,
		double aperture,
		double focus_dist
	) {
		double theta = degrees_to_radians(vertical_fov);
		double height = tan(theta / 2);
		double viewport_height = 2.0 * height;
		double viewport_width = aspect_ratio * viewport_height;

		w = normalize(look_from - look_at);
		u = normalize(cross(vup, w));
		v = cross(w, u);

		origin = look_from;
		horizontal = focus_dist *  viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		lower_left_corner = origin - (horizontal / 2) - (vertical / 2)
			- (focus_dist * w);
		
		lens_radius = aperture / 2;
	}

	Ray get_ray(double s, double t) const {
		Tuple3d rand = lens_radius * random_in_unit_disk();
		Tuple3d offset = (u * rand.x()) + (v * rand.y());

		return Ray(
			origin + offset,
			lower_left_corner + (s * horizontal) + (t * vertical) - origin - offset
		);
	}

private:
	point3d origin;
	point3d lower_left_corner;
	Tuple3d horizontal;
	Tuple3d vertical;
	Tuple3d u, v, w;
	double lens_radius;
};
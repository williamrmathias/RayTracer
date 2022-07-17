#include "Ray.h"

#pragma once

struct hit_record {
	point3d p;
	Tuple3d normal;
	double t;
	bool front_face;

	void set_face_normal(const Ray& ray, const Tuple3d& outward_normal) {
		front_face = dot(ray.get_direction(), outward_normal) < 0;
		if (front_face) { normal = outward_normal; }
		else { normal = -outward_normal; }
	}
};

class Surface {
public:
	virtual bool hit(const Ray& ray, double t_min,
		double t_max, hit_record& record) const = 0;
};
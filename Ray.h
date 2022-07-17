#include "Tuple3d.h"

#pragma once

class Ray {
private:
	point3d origin;
	Tuple3d direction;

public:
	//default ctor
	Ray() {}

	//ctor
	Ray(const point3d& origin, const Tuple3d& direction)
		: origin(origin), direction(direction) {}

	// origin accessor
	point3d get_origin() const { return origin; }

	// direction accessor
	Tuple3d get_direction() const { return direction; }

	// gives the position at time t
	point3d at(double t) const {
		return origin + t * direction;
	}
};
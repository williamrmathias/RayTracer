#include <cmath>
#include<iostream>

#include "utility.h"

#pragma once

class Tuple3d{
	// a class representing a 3d vector
	// can make a point in space or an RGB pixel
private:
	double coords[3];

public:
	// default ctor
	Tuple3d()
		: coords{ 0, 0, 0 } {}

	//ctor
	Tuple3d(double x, double y, double z) 
		: coords{ x, y, z } {}

	// underlying array access
	double operator[](int i) const {
		if (i >= 0 && i < 3) {
			return coords[i];
		}
		else {
			std::cerr << "ERROR: accessed Tuple3d out of bounds" << '\n';
			return coords[0];
		}
	}

	double& operator[](int i) {
		if (i >= 0 && i < 3) {
			return coords[i];
		}
		else {
			std::cerr << "ERROR: accessed Tuple3d out of bounds" << '\n';
			return coords[0];
		}
	}

	double x() const { return coords[0]; }
	double y() const { return coords[1]; }
	double z() const { return coords[2]; }

	// negative vector
	Tuple3d operator-() const {
		return Tuple3d(-coords[0], -coords[1], -coords[2]);
	}

	// vector addition
	Tuple3d& operator+=(const Tuple3d& v) {
		coords[0] += v.coords[0];
		coords[1] += v.coords[1];
		coords[2] += v.coords[2];
		return *this;
	}

	// vector pairwise multiplication
	Tuple3d& operator*=(const Tuple3d& v) {
		coords[0] *= v.coords[0];
		coords[1] *= v.coords[1];
		coords[2] *= v.coords[2];
		return *this;
	}

	// vector scaling
	Tuple3d& operator*=(const double k) {
		coords[0] *= k;
		coords[1] *= k;
		coords[2] *= k;
		return *this;
	}

	// vector scaling (inverted)
	Tuple3d& operator/=(const double k) {
		return *this *= (1 / k);
	}

	// vector length squared
	double length_squared() const {
		return coords[0] * coords[0] + coords[1] * coords[1] 
			+ coords[2] * coords[2];
	}

	// vector length
	double length() const {
		return std::sqrt(this->length_squared());
	}

	// random vectors
	static Tuple3d random() {
		return Tuple3d(random_double(), random_double(), random_double());
	}

	static Tuple3d random(double min, double max) {
		return Tuple3d(random_double(min, max),
			random_double(min, max), random_double(min, max));
	}

	bool near_zero() const {
		// Returns true if the vector is close to zero in all dimensions.
		const double s = 1e-8;
		return (fabs(coords[0]) < s) &&
			(fabs(coords[1]) < s) &&
			(fabs(coords[2]) < s);
	}
};

using point3d = Tuple3d; // point in 3d space
using color = Tuple3d; // RGB color

// Tuple3d utility

// printing out Tuple3d
std::ostream& operator<<(std::ostream& out, const Tuple3d& v) {
	return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

// vector addition
Tuple3d operator+(const Tuple3d& v, const Tuple3d& w) {
	return Tuple3d(v.x() + w.x(), v.y() + w.y(), v.z() + w.z());
}

// vector subtraction
Tuple3d operator-(const Tuple3d& v, const Tuple3d& w) {
	return Tuple3d(v.x() - w.x(), v.y() - w.y(), v.z() - w.z());
}

// vector pairwise multiplication
Tuple3d operator*(const Tuple3d& v, const Tuple3d& w) {
	return Tuple3d(v.x() * w.x(), v.y() * w.y(), v.z() * w.z());
}

// vector scalar multiplication
Tuple3d operator*(const Tuple3d& v, const double k) {
	return Tuple3d(v.x() * k, v.y() * k, v.z() * k);
}
Tuple3d operator*(const double k, const Tuple3d& v) {
	return v * k;
}

// vector scalar division
Tuple3d operator/(const Tuple3d& v, const double k) {
	return v * (1/k);
}

// vector dot product
double dot(const Tuple3d& v, const Tuple3d& w) {
	return v.x() * w.x() + v.y() * w.y() + v.z() * w.z();
}

// vector cross product
Tuple3d cross(const Tuple3d& v, const Tuple3d& w) {
	return Tuple3d(v.y() * w.z() - v.z() * w.y(),
		v.z() * w.x() - v.x() * w.z(),
		v.x() * w.y() - v.y() * w.x());
}

// make vector a unit vector in same direction
Tuple3d normalize(Tuple3d v) {
	return v / v.length();
}

// random vector in sphere
Tuple3d random_in_unit_sphere() {
	while (true) {
		Tuple3d p = Tuple3d::random(-1, 1);
		if (p.length_squared() >= 1) { continue; }
		return p;
	}
}

// random normalized vector in sphere
Tuple3d random_normal_vector() {
	return normalize(random_in_unit_sphere());
}

// random vector in hemisphere
Tuple3d random_in_hemi(const Tuple3d& normal) {
	Tuple3d in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0) {
		return in_unit_sphere;
	}
	else {
		return -in_unit_sphere;
	}
}

// tuple reflection
Tuple3d reflect(const Tuple3d& v, const Tuple3d& norm) {
	return v - 2 * dot(v, norm) * norm;
}

// light refraction
Tuple3d refract(const Tuple3d& ray_in, const Tuple3d& n, double eta_ratio) {
	double cos_theta = fmin(dot(-ray_in, n), 1.0);
	Tuple3d ray_out_perp = eta_ratio * (ray_in + cos_theta * n);
	Tuple3d ray_out_para = -sqrt(
		fabs(1.0 - ray_out_perp.length_squared())) * n;
	return ray_out_perp + ray_out_para;
}
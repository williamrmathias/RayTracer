#include "utility.h"
#include "Ray.h"
#include "Tuple3d.h"
#include "Surface.h"

#pragma once

struct hit_record;

class Material {
public:
	virtual bool scatter(
		const Ray& ray_in, const hit_record& record, color& attenuation,
		Ray& scattered
	) const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(const color& a) : albedo(a) {}

	virtual bool scatter(
		const Ray& ray_in, const hit_record& record, color& attenuation,
		Ray& scattered
	) const override {
		Tuple3d scatter_direction = record.normal + random_normal_vector();

		// Catch degenerate scatter
		if (scatter_direction.near_zero()) {
			scatter_direction = record.normal;
		}

		scattered = Ray(record.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

private:
	color albedo;
};

class Metal : public Material {
public:
	Metal(const color& a, double f) : albedo(a), fuzz(clamp(f, 0, 1)) {}

	virtual bool scatter(
		const Ray& ray_in, const hit_record& record, color& attenuation,
		Ray& scattered
	) const override {
		Tuple3d reflected = reflect(normalize(ray_in.get_direction()),
			record.normal);
		scattered = Ray(record.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.get_direction(), record.normal) > 0.0);
	}

private:
	color albedo;
	double fuzz;
};
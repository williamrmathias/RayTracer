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

class Dielectric : public Material {
public:
	Dielectric(double index) : refrac_idx(index) {}

	virtual bool scatter(
		const Ray& ray_in, const hit_record& record, color& attenuation,
		Ray& scattered
	) const override {
		attenuation = color(1.0, 1.0, 1.0);

		double refrac_ratio;
		if (record.front_face) {
			refrac_ratio = 1.0 / this->refrac_idx;
		}
		else {
			refrac_ratio = this->refrac_idx;
		}

		Tuple3d unit_direction = normalize(ray_in.get_direction());

		double cos_theta = fmin(dot(-unit_direction, record.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		Tuple3d direction;

		if (refrac_ratio * sin_theta > 1.0 ||
			reflectance(cos_theta, refrac_ratio) > random_double()) {
			direction = reflect(unit_direction, record.normal);
		}
		else {
			direction = refract(unit_direction, record.normal, refrac_ratio);
		}

		scattered = Ray(record.p, direction);
		return true;
	}

private:
	double refrac_idx;
	static double reflectance(double cosine, double refrac_idx) {
		// Schlick's approximation for reflectance
		double r0 = (1 - refrac_idx) / (1 + refrac_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
};
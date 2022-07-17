#include "Surface.h"
#include "utility.h"

#pragma once

class Sphere : public Surface {
public:
	//default ctor
	Sphere() {}
	// ctor
	Sphere(point3d c, double r, std::shared_ptr<Material> material) 
		: center(c), radius(r), material_ptr(material) {}

	virtual bool hit(
		const Ray& ray, double t_min, double t_max, hit_record& record) const override;
	
private:
	point3d center;
	double radius;
	std::shared_ptr<Material> material_ptr;
};

bool Sphere::hit(
	const Ray& ray, double t_min, double t_max, hit_record& record) const {
	Tuple3d oc = ray.get_origin() - this->center;
	double a = ray.get_direction().length_squared();
	double half_b = dot(oc, ray.get_direction());
	double c = oc.length_squared() - (this->radius * this->radius);
	double discriminant = (half_b * half_b) - (a * c);

	if (discriminant < 0) {return false;}
	double root = (-half_b - std::sqrt(discriminant)) / a;
	if (root < t_min || root > t_max) {
		root = (-half_b + std::sqrt(discriminant)) / a;
		if (root < t_min || root > t_max) { return false; }
	}

	record.p = ray.at(root);
	record.t = root;
	Tuple3d outward_norm = (record.p - this->center) / this->radius;
	record.set_face_normal(ray, outward_norm);
	record.material_ptr = this->material_ptr;

	return true;
}
#include "Surface.h"
#include <vector>
#include<memory>

#pragma once

class SurfaceList : public Surface {
public:
	//default ctor
	SurfaceList() {}

	//ctor one object
	SurfaceList(std::shared_ptr<Surface> object) { 
		this->objects.push_back(object);
	}
	//ctor many objects
	SurfaceList(std::vector<std::shared_ptr<Surface>> list) {
		this->objects = list;
	}

	void clear() {
		this->objects.clear();
	}

	void add(std::shared_ptr<Surface> object) { 
		this->objects.push_back(object);
	}

	virtual bool hit(
		const Ray& ray, double t_min,
		double t_max, hit_record& record) const override;

private:
	std::vector<std::shared_ptr<Surface>> objects;
};

bool SurfaceList::hit(const Ray& ray, double t_min,
	double t_max, hit_record& record) const {
	hit_record temp_rec;
	bool anything_hit = false;
	double closest = t_max; //maximum possible distance

	for (const auto& object : objects) {
		if (object->hit(ray, t_min, closest, temp_rec)) {
			anything_hit = true;
			closest = temp_rec.t;
			record = temp_rec;
		}
	}

	return anything_hit;
}
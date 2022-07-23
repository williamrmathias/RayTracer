#include <iostream>
#include "utility.h"
#include "Color.h"
#include "Ray.h"
#include "Tuple3d.h"
#include "Sphere.h"
#include "Surface.h"
#include "SurfaceList.h"
#include "Camera.h"
#include "Material.h"

color ray_color(const Ray& ray, const SurfaceList& world, int depth) {
	hit_record record;

	// If we exceed the recursion limit
	if (depth <= 0) {
		return color(0, 0, 0);
	}

	if (world.hit(ray, 0.001, infinity, record)) {
		Ray scattered;
		color attenuation;
		if (record.material_ptr->scatter(ray, record,
			attenuation, scattered)) {
			return attenuation * ray_color(scattered, world, depth - 1);
		}
		return color(0, 0, 0);
	}

	Tuple3d unit_direction = normalize(ray.get_direction());
	double t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

	// image
	const double ASPECT_RATIO = 16.0 / 9.0;
	const int IMAGE_WIDTH = 400;
	const int IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH / ASPECT_RATIO);
	const int num_samples = 100;
	const int max_depth = 50;

	// camera

	Camera camera(
		20.0,
		ASPECT_RATIO,
		point3d(-2, 2, 1),
		point3d(0, 0, -1),
		Tuple3d(0, 1, 0)
	);

	// world surface

	SurfaceList world;

	auto material_ground = std::make_shared<Lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<Lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = std::make_shared<Dielectric>(1.5);
	auto material_right = std::make_shared<Metal>(color(0.8, 0.6, 0.2), 0.0);

	world.add(std::make_shared<Sphere>(point3d(0.0, -100.5, -1.0),
		100.0, material_ground));
	world.add(std::make_shared<Sphere>(point3d(0.0, 0.0, -1.0),
		0.5, material_center));
	world.add(std::make_shared<Sphere>(point3d(-1.0, 0.0, -1.0),
		0.5, material_left));
	world.add(std::make_shared<Sphere>(point3d(-1.0, 0.0, -1.0),
		-0.45, material_left));
	world.add(std::make_shared<Sphere>(point3d(1.0, 0.0, -1.0),
		0.5, material_right));


	

	// render

	std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

	for (int j = IMAGE_HEIGHT - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < IMAGE_WIDTH; ++i) {
			color pixel(0, 0, 0);
			for (int s = 0; s < num_samples; ++s) {
				double u = (i + random_double()) / (IMAGE_WIDTH - 1);
				double v = (j + random_double()) / (IMAGE_HEIGHT - 1);
				Ray ray = camera.get_ray(u, v);
				pixel += ray_color(ray, world, max_depth);
			}
			print_color(std::cout, pixel, num_samples);
		}
	}

	std::cerr << "\nDone.\n";
}
#include <iostream>
#include "utility.h"
#include "Color.h"
#include "Ray.h"
#include "Tuple3d.h"
#include "Sphere.h"
#include "Surface.h"
#include "SurfaceList.h"
#include "Camera.h"

color ray_color(const Ray& ray, const SurfaceList& world) {
	hit_record record;
	if (world.hit(ray, 0, infinity, record)) {
		return 0.5 * color(record.normal + color(1, 1, 1));
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

	// camera

	Camera camera;

	// world surface
	SurfaceList world;
	world.add(std::make_shared<Sphere>(Tuple3d(0, 0, -1), 0.5));
	world.add(std::make_shared<Sphere>(Tuple3d(0, -100.5, -1), 100));

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
				pixel += ray_color(ray, world);
			}
			print_color(std::cout, pixel, num_samples);
		}
	}

	std::cerr << "\nDone.\n";
}
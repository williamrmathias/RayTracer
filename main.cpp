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

SurfaceList random_world() {
	SurfaceList world;

	auto ground_material = std::make_shared<Lambertian>(color(0.5, 0.5, 0.5));
	world.add(std::make_shared<Sphere>(point3d(0, -1000, 0),
		1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			double choose_material = random_double();
			point3d center(a + 0.9 * random_double(), 0.2,
				b + 0.9 * random_double());

			if ((center - point3d(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<Material> sphere_material;

				if (choose_material < 0.8) {
					// Lambertian
					color albedo = color::random() * color::random();
					sphere_material = std::make_shared<Lambertian>(albedo);
					world.add(std::make_shared<Sphere>(center, 0.2,
						sphere_material));
				}
				else if (choose_material < 0.95) {
					// Metal
					color albedo = color::random(0.5, 1);
					double fuzz = random_double(0, 0.5);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
					world.add(std::make_shared<Sphere>(center, 0.2,
						sphere_material));
				}
				else {
					// Glass
					sphere_material = std::make_shared<Dielectric>(1.5);
					world.add(std::make_shared<Sphere>(center, 0.2,
						sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(1.5);
	world.add(std::make_shared<Sphere>(point3d(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<Lambertian>(color(0.4, 0.2, 0.1));
	world.add(std::make_shared<Sphere>(point3d(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(std::make_shared<Sphere>(point3d(4, 1, 0), 1.0, material3));

	return world;
}

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
	const double ASPECT_RATIO = 3.0 / 2.0;
	const int IMAGE_WIDTH = 1200;
	const int IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH / ASPECT_RATIO);
	const int num_samples = 250;
	const int max_depth = 50;

	// camera

	point3d look_from(13, 2, 3);
	point3d look_at(0, 0, 0);
	Tuple3d vup(0, 1, 0);
	double dist_to_focus = 10.0;
	double aperture = 0.1;

	Camera camera(look_from, look_at, vup, 20.0, ASPECT_RATIO,
		aperture, dist_to_focus);

	// world surface

	SurfaceList world = random_world();
	

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
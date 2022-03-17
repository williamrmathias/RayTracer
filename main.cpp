#include <iostream>
#include "Color.h"
#include "Ray.h"
#include "Tuple3d.h"

bool hit_sphere(const point3d& center, double radius, const Ray& ray) {
	Tuple3d oc = ray.get_origin() - center;
	double a = dot(ray.get_direction(), ray.get_direction());
	double b = 2.0 * dot(oc, ray.get_direction());
	double c = dot(oc, oc) - (radius * radius);
	double discriminant = (b * b) - (4 * a * c);
	return discriminant > 0;
}

color ray_color(const Ray& ray) {
	if (hit_sphere(point3d(0, 0, -2), 0.5, ray)) {
		// this colors the sphere red
		return color(1, 0, 0);
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

	// camera

	double viewport_height = 2.0;
	double viewport_width = ASPECT_RATIO * viewport_height;
	double focal_length = 1.0;

	point3d origin = point3d(0, 0, 0);
	Tuple3d horizontal = Tuple3d(viewport_width, 0, 0);
	Tuple3d vertical = Tuple3d(0, viewport_height, 0);
	point3d lower_left_corner = origin - (horizontal / 2) - 
		(vertical / 2) - Tuple3d(0, 0, focal_length);

	// render

	std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

	for (int j = IMAGE_HEIGHT - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < IMAGE_WIDTH; ++i) {
			double u = double(i) / (IMAGE_WIDTH - 1);
			double v = double(j) / (IMAGE_HEIGHT - 1);
			Ray ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			color pixel_color = ray_color(ray);
			print_color(std::cout, pixel_color);
		}
	}

	std::cerr << "\nDone.\n";
}
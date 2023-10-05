#include "rtweekend.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>

/*

a=b⋅b
b=2b⋅(A−C)
c=(A−C)⋅(A−C)−r*r
*/

color ray_color(const ray& r, const hittable& world) {
	hit_record rec;
	if (world.hit(r, interval(0, infinity), rec)) {
		return 0.5 * (rec.normal + color(1, 1, 1));
	}
	
	vec3 unit_direction = unit_vector(r.direction());
	auto a = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main()
{
	//Image
	const auto ratio = 16.0 / 9.0;		//宽高比
	const int image_width = 400;		//宽度
	int image_height = static_cast<int>(image_width / ratio);		//高度
	image_height = (image_height < 1) ? 1 : image_height;

	//object
	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	//camera
	auto camera_center = point3(0, 0, 0);

	auto focal_length = 1.0;
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

	
	auto viewport_horizontal = vec3(viewport_width, 0, 0);
	auto viewport_vertical = vec3(0, -viewport_height, 0);

	auto pixel_delta_horizontal = viewport_horizontal / image_width;
	auto pixel_delta_vertical = viewport_vertical / image_height;

	point3 viewport_up_left = point3(camera_center - viewport_horizontal / 2 - viewport_vertical / 2 - point3(0, 0, focal_length));

	auto pixel_loc = viewport_up_left + 0.5 * (pixel_delta_horizontal + pixel_delta_vertical);

	std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int j = 0; j < image_height; j++) {
		std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

		for (int i = 0; i < image_width; i++) {
			auto pixel_center = pixel_loc + (i * pixel_delta_horizontal) + (j * pixel_delta_vertical);
			auto ray_direction = pixel_center - camera_center;

			ray r(camera_center, ray_direction);
			color pixel_color = ray_color(r, world);

			write_color(std::cout, pixel_color);
		}
	}
	std::clog << "\rDone.                 \n";
}
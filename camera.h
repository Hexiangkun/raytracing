#pragma once
#include "rtweekend.h"
#include "vec3.h"
#include "hittable.h"


class camera
{
public:
	camera(int _image_width, double ratio, int _samples):image_width(_image_width), aspect_ratio(ratio), samples_per_pixel(_samples)
	{
		initialize();
	}

	void render(const hittable& world)
	{

		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (int j = 0; j < image_height; j++) {
			std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

			for (int i = 0; i < image_width; i++) {
				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; sample++) {
					ray r = get_ray(i, j);
					pixel_color += ray_color(r, world);
				}

				write_color(std::cout, pixel_color, samples_per_pixel);
			}
		}
		std::clog << "\rDone.                 \n";
	}


private:
	void initialize()
	{
		image_height = static_cast<int>(image_width / aspect_ratio);
		image_height = image_height < 1 ? 1 : image_height;

		camera_center = point3(0, 0, 0);

		auto focal_length = 1.0;
		auto viewport_height = 2.0;
		auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);


		auto viewport_u = vec3(viewport_width, 0, 0);
		auto viewport_v = vec3(0, -viewport_height, 0);

		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		auto viewport_upper_left = camera_center - viewport_u / 2 - viewport_v / 2 - vec3(0, 0, focal_length);

		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	}

	color ray_color(const ray& r, const hittable& world) {
		hit_record rec;
		if (world.hit(r, interval(0, infinity), rec)) {
			return 0.5 * (rec.normal + color(1, 1, 1));
		}

		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}

	void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {
		auto scale = 1.0 / samples_per_pixel;
		auto r = scale * pixel_color.x();
		auto g = scale * pixel_color.y();
		auto b = scale * pixel_color.z();

		static const interval intensity(0.000, 0.999);
		out << static_cast<int>(256 * intensity.clamp(r)) << ' '
			<< static_cast<int>(256 * intensity.clamp(g)) << ' '
			<< static_cast<int>(256 * intensity.clamp(b)) << '\n';
	}

	ray get_ray(int i, int j) const
	{
		auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);

		auto pixel_sample = pixel_center + pixel_sample_square();

		auto ray_origin = camera_center;
		auto ray_direction = pixel_center - ray_origin;

		ray r(ray_origin, ray_direction);
		return r;
	}

	vec3 pixel_sample_square() const
	{
		auto px = -0.5 + random_double();
		auto py = -0.5 + random_double();
		return (px * pixel_delta_u) + (py * pixel_delta_v);
	}

private:
	double aspect_ratio;
	int image_width;
	int image_height;
	point3 camera_center;
	point3 pixel00_loc;	//location of pixel 0,0
	vec3 pixel_delta_u;		//offset to pixel to the right
	vec3 pixel_delta_v;		//offset to pixel to below
	int samples_per_pixel;
};
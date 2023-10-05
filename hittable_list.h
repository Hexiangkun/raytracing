#pragma once
#include "hittable.h"
#include <memory>
#include <vector>

class hittable_list : public hittable
{
public:
	std::vector<std::shared_ptr<hittable>> objects;

public:
	hittable_list(){}
	~hittable_list() {}

	void clear()
	{
		objects.clear();
	}

	void add(std::shared_ptr<hittable> object)
	{
		objects.push_back(object);
	}

	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const
	{
		hit_record temp_record;
		bool hit_anything = false;

		auto closest_so_far = ray_t.max;

		for (const auto& object : objects) {
			if (object->hit(r, interval(ray_t.min, closest_so_far), temp_record)) {
				hit_anything = true;
				closest_so_far = temp_record.t;
				rec = temp_record;
			}
		}
		return hit_anything;
	}
};
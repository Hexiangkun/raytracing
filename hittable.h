#pragma once
#include "ray.h"
#include "interval.h"
#include <memory>

class material;

struct hit_record
{
	point3 p;		//光线和物体相交点
	vec3 normal;	//交点的法线
	double t;		//光线的长度
	bool front_face;	//法向量是否和射线成锐角
	shared_ptr<material> mat;

	void set_face_normal(const ray& r, const vec3& outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable
{
public:
	hittable() = default;
	virtual ~hittable() = default;
	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

};

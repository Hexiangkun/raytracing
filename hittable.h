#pragma once
#include "ray.h"
#include "interval.h"
#include <memory>

class material;

struct hit_record
{
	point3 p;		//���ߺ������ཻ��
	vec3 normal;	//����ķ���
	double t;		//���ߵĳ���
	bool front_face;	//�������Ƿ�����߳����
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

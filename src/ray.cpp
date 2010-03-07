#include "ray.hpp"
#include <iostream>

using namespace std;

Ray::Ray()
:	origin(Point3D(0, 0, 0)),
	dir(Vector3D(0, 0, 1)),
	hit(false),
	n(Vector3D(0, 0, 1)),
	name(""),
	material(NULL),
	hitPos(Point3D(0, 0, 0)),
	secondaryRay(false)
	
{
	t = 0;
}
Ray::Ray(const Point3D& origin, const Vector3D& dir)
:	origin(origin),
	dir(dir),
	hit(false),
	n(Vector3D(0, 0, 1)),
	name(""),
	material(NULL),
	hitPos(Point3D(0, 0, 0)),
	secondaryRay(false)
{
	t = 0;
}


Ray::Ray(const Ray& ray)
:	origin(ray.origin),
	dir(ray.dir),
	hit(false),
	n(ray.n),
	name(ray.name),
	material(ray.material),
	hitPos(ray.hitPos),
	secondaryRay(false)
{}

Ray::~Ray()
{
	
}

std::ostream& operator<<(std::ostream& out, const Ray& ray)
{
	std::cerr << "Ray eye: " << ray.origin << "\tdir:" << ray.dir <<std::endl;
	return out;
}

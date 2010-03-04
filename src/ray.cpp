#include "ray.hpp"
#include <iostream>

using namespace std;

Ray::Ray()
:	origin(Point3D(0, 0, 0)),
	dir(Vector3D(0, 0, 1)),
	hit(false),
	secondaryRay(false)
{
	t = 0;
}
Ray::Ray(const Point3D& origin, const Vector3D& dir)
:	origin(origin),
	dir(dir),
	hit(false),
	secondaryRay(false)
{
	t = 0;
}

Ray::Ray(const Ray& ray)
:	origin(ray.origin),
	dir(ray.dir),
	hit(false),
	secondaryRay(false)
{}

Ray::~Ray()
{
	
}
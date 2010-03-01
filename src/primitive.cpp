#include "primitive.hpp"
#include "polyroots.hpp"

Primitive::~Primitive()
{
}

bool Sphere::hit(Ray& ray, double epsilon)
{
	return true;
}
Sphere::~Sphere()
{
}

Cube::~Cube()
{
}

bool Cube::hit(Ray& ray, double epsilon)
{
	return true;
}

NonhierSphere::~NonhierSphere()
{
}

bool NonhierSphere::hit(Ray& ray, double epsilon)
{
	double a, b, c, t;
	t = 0;

	Vector3D temp;
	temp = ray.origin - m_pos;
	
	// Coefficients of basic quadratic equation at^2 + bt + c
	a = ray.dir.dot(ray.dir);
	b = 2.0 * temp.dot(ray.dir);
	c = temp.dot(temp) - m_radius * m_radius;

	double roots[2];
	int ret = quadraticRoots(a, b, c, roots);

	if (ret > 0)
	{
		//std::cerr << "Intersect: " << (ray.origin + roots[0] * ray.dir) << "\t" << (ray.origin + roots[1] * ray.dir) << "\n";

		if (roots[0] < epsilon)
			roots[0] = LONG_MAX;
			
		if (roots[1] < epsilon)
			roots[1] = LONG_MAX;
		
		
		t = min(roots[0], roots[1]);

		if (ray.isHit() && t > ray.t)
		{
			// This object is closer to the origin so we will update our ray
			return false;
		}
			
		ray.n = Vector3D(ray.origin[0] + t * ray.dir[0] - m_pos[0], ray.origin[1] + t * ray.dir[1] - m_pos[1], ray.origin[2] + t * ray.dir[2] - m_pos[2]);
		ray.n.normalize();
		ray.t = t;			
		ray.setHit(true);
		
		return true;
	}
	else
	{
		// Nothing was hit
	}
	return false;
}

NonhierBox::~NonhierBox()
{
}

bool NonhierBox::hit(Ray& ray, double epsilon)
{
	return true;
}
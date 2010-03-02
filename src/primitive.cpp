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
		
		// If the root doesn't exist AND the ray hasn't
		// hit anything yet then we ignore this solution
		if (t == LONG_MAX && !ray.isHit())
		{
			t = 0;
			return false;
		}
		
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
	Vector3D normal(1,0,0);
	
	double numer, denom;
	Point3D pointOnPlane = m_pos;
	for (int i = 0;i<3;i++)
	{
		normal[0] = 0;
		normal[1] = 0;
		normal[2] = 0;
		if (i == 0)
			normal[0] = 1;
		else if (i == 1)
			normal[1] = 1;
		else
		{
			normal[2] = 1;
			pointOnPlane[0] += m_size;
			pointOnPlane[1] += m_size;
			pointOnPlane[2] += m_size;
		} 

		
		double t = (pointOnPlane - ray.origin).dot(normal) / (normal.dot(ray.dir));
		if (t > epsilon)
		{
			// ray hits plane
			
			// Check if hitting the other side of the box is closer
			normal = -1 * normal;
			double t2 = (pointOnPlane - ray.origin).dot(normal) / (normal.dot(ray.dir));
			if (t > t2 && t2 > epsilon)
				t = t2;
			
			if (ray.isHit() && t > ray.t)
			{
				// This object is closer to the origin so we will update our ray
				return false;
			}
			if (t > 1 || t < 0)
				return false;			

			std::cerr << "Hit\t" << t << "\n" ;
			ray.n = normal;
			ray.t = t;			
			ray.setHit(true);

			return true;
		}
	}
	return false;
}
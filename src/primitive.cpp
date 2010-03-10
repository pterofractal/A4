#include "primitive.hpp"
#include "polyroots.hpp"

#include "util.hpp"

pthread_mutex_t mutex3;

Primitive::~Primitive()
{
}

bool Sphere::hit(Ray& ray, double epsilon)
{
	Point3D m_pos(0, 0, 0);
	int m_radius = 1;
	NonhierSphere simpleSphere(m_pos, m_radius);
	
	return simpleSphere.hit(ray, epsilon);
}
Sphere::~Sphere()
{
}

Cube::~Cube()
{
}

bool Cube::hit(Ray& ray, double epsilon)
{
		Point3D m_pos(0.0, 0.0, 0.0);
		int m_size = 1;
		NonhierBox simpleBox(m_pos, m_size);
		return simpleBox.hit(ray, epsilon);
}

NonhierSphere::~NonhierSphere()
{
}

bool NonhierSphere::hit(Ray& ray, double epsilon)
{
	double a, b, c, t;
	t = 0;
	Vector3D temp = ray.origin - m_pos;
	
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
	Vector3D normal[6];
	
	// The normals for each face
	normal[0] = Vector3D(0, 0, 1);
	normal[1] = Vector3D(0, 0, -1);
	normal[2] = Vector3D(0, 1, 0);
	normal[3] = Vector3D(0, -1, 0);
	normal[4] = Vector3D(1, 0, 0);
	normal[5] = Vector3D(-1, 0, 0);
	
	Point3D hitPos;
	bool ret = false;
	Ray duplicateRay = ray;

	// Intersect with front face
	Plane frontPlane(m_pos, normal[0]);
	frontPlane.hit(duplicateRay, epsilon);
	hitPos = duplicateRay.getHitPos();
	if (m_pos[0] < hitPos[0] && m_pos[1] < hitPos[1] && fabs(m_pos[2] - hitPos[2]) < epsilon &&
	    m_pos[0] + m_size > hitPos[0] && m_pos[1] + m_size > hitPos[1])
	{
		ret = true;
		ray = duplicateRay;
	}
	
	
	duplicateRay = ray;
	// Intersect with Botom face
	Plane bottomPlane(m_pos, normal[3]);
	bottomPlane.hit(duplicateRay, epsilon);
	hitPos = duplicateRay.getHitPos();
	if (m_pos[0] < hitPos[0] && fabs(m_pos[1] - hitPos[1]) < epsilon && m_pos[2] < hitPos[2] &&
	    m_pos[0] + m_size > hitPos[0] && m_pos[2] + m_size > hitPos[2])
	{
		ray = duplicateRay;
		ret = true;
	}

	duplicateRay = ray;
	// Intersect with left side face
	Plane leftPlane(m_pos, normal[5]);
	leftPlane.hit(duplicateRay, epsilon);
	hitPos = duplicateRay.getHitPos();
	if (fabs(m_pos[0] - hitPos[0]) < epsilon && m_pos[1] < hitPos[1] && m_pos[2] < hitPos[2] &&
	    m_pos[1] + m_size > hitPos[1] && m_pos[2] + m_size > hitPos[2])
	{
		ray = duplicateRay;
		ret = true;
	}
	
	Point3D oppositeCorner(m_size + m_pos[0], m_size + m_pos[1], m_size + m_pos[2]);
	
	duplicateRay = ray;
	// Intersect with the back face	
	Plane backPlane(oppositeCorner, normal[1]);
	backPlane.hit(duplicateRay, epsilon);
	hitPos = duplicateRay.getHitPos();
	if (oppositeCorner[0] > hitPos[0] && oppositeCorner[1] > hitPos[1] && fabs(oppositeCorner[2] - hitPos[2]) < epsilon &&
	    oppositeCorner[0] - m_size < hitPos[0] && oppositeCorner[1] - m_size < hitPos[1])
	{
		ray = duplicateRay;
		ret = true;
	}
	
	duplicateRay = ray;
	// Intersect with top face
	Plane topPlane(oppositeCorner, normal[2]);
	topPlane.hit(duplicateRay, epsilon);
	hitPos = duplicateRay.getHitPos();
	if (oppositeCorner[0] > hitPos[0] && fabs(oppositeCorner[1] - hitPos[1]) < epsilon && oppositeCorner[2] > hitPos[2] &&
	    oppositeCorner[0] - m_size < hitPos[0] && oppositeCorner[2] - m_size < hitPos[2])
	{
		ray = duplicateRay;
		ret = true;
	}
	
	duplicateRay = ray;
	// Intersect with other side face		
	Plane rightPlane(oppositeCorner, normal[4]);
	rightPlane.hit(duplicateRay, epsilon);
	hitPos = duplicateRay.getHitPos();
	if (fabs(oppositeCorner[0] - hitPos[0]) < epsilon && oppositeCorner[1] > hitPos[1] && oppositeCorner[2] > hitPos[2] &&
	    oppositeCorner[2] - m_size < hitPos[2] && oppositeCorner[1] - m_size < hitPos[1])
	{
		ray = duplicateRay;
		ret = true;
	}

	return ret;
}


Plane::Plane(Point3D pointOnPlane, Vector3D n)
{
	pos = pointOnPlane;
	normal = n;
}

Plane::~Plane()
{
	
}
bool Plane::hit(Ray& ray, double epsilon)
{
	double numer, denom;
	numer = (pos - ray.origin).dot(normal);
	denom = ray.dir.dot(normal);
	double t = (pos - ray.origin).dot(normal) / (ray.dir.dot(normal));
	
	if (ray.isHit() && ray.t < t)
	{
		return false;
	}
	
	t = numer / denom;
	// std::cout << "denom\t" << denom << "\n";
	if (t > epsilon)
	{
		ray.t = t;
		ray.n = normal;
		ray.setHit(true);
		return true;
	}
	return false;
}

#include "primitive.hpp"
#include "polyroots.hpp"
#include "util.hpp"

Primitive::~Primitive()
{
}

bool Sphere::hit(Ray& ray, double epsilon)
{
	double a, b, c, t;
	t = 0;
	Point3D m_pos(0, 0, 0);
	int m_radius = 1;

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
		double ox = ray.origin[0]; 
		double oy = ray.origin[1]; 
		double oz = ray.origin[2];
		double dx = ray.dir[0]; 
		double dy = ray.dir[1]; 
		double dz = ray.dir[2];

		double tx_min, ty_min, tz_min;
		double tx_max, ty_max, tz_max; 

		double a = 1.0 / dx;
		if (a >= 0) {
			tx_min = (m_pos[0] - ox) * a;
			tx_max = (m_pos[0] + m_size - ox) * a;
		}
		else {
			tx_min = (m_pos[0] + m_size - ox) * a;
			tx_max = (m_pos[0] - ox) * a;
		}

		double b = 1.0 / dy;
		if (b >= 0) {
			ty_min = (m_pos[1] - oy) * b;
			ty_max = (m_pos[1] + m_size - oy) * b;
		}
		else {
			ty_min = (m_pos[1] + m_size - oy) * b;
			ty_max = (m_pos[1] - oy) * b;
		}

		double c = 1.0 / dz;
		if (c >= 0) {
			tz_min = (m_pos[2] - oz) * c;
			tz_max = (m_pos[2] + m_size - oz) * c;
		}
		else {
			tz_min = (m_pos[2] + m_size - oz) * c;
			tz_max = (m_pos[2] - oz) * c;
		}

		double t0, t1;
		int face_in, face_out;
		double tmin = LONG_MAX;
		// find largest entering t value

		if (tx_min > ty_min) {
			t0 = tx_min;
			face_in = (a >= 0.0) ? 0 : 3;
		}
		else {
			t0 = ty_min;
			face_in = (b >= 0.0) ? 1 : 4;
		}

		if (tz_min > t0) {
			t0 = tz_min;
			face_in = (c >= 0.0) ? 2 : 5;
		}

		// find smallest exiting t value

		if (tx_max < ty_max) {
			t1 = tx_max;
			face_out = (a >= 0.0) ? 3 : 0;
		}
		else {
			t1 = ty_max;
			face_out = (b >= 0.0) ? 4 : 1;
		}

		if (tz_max < t1) {
			t1 = tz_max;
			face_out = (c >= 0.0) ? 5 : 2;
		}
		
		//std::cout << "tx:\t" << tx_max << "\tt1: "<< tx_min << "ty:\t" << ty_max << "\tt1: "<< ty_min<< "tx:\t" << tz_max << "\tt1: "<< tz_min << "\n";
		
		if (t0 < t1 && t1 > epsilon) {  // condition for a hit
			if (t0 > epsilon) {
				tmin = t0;  			// ray hits outside surface

				ray.n = get_normal_for_cube(face_in);
			}
			else {
				tmin = t1;				// ray hits inside surface

				ray.n = get_normal_for_cube(face_out);
			}

			if (tmin == LONG_MAX && !ray.isHit())
			{
				tmin = 0;
				return false;
			}

			if (ray.isHit() && tmin > ray.t)
				return false;

			// std::cout << ray;
			ray.t = tmin;
			ray.setHit(true);
			return (true);
		}

		return false;
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
/*	Vector3D normal[6];
		
	normal[0] = Vector3D(0, 0, 1);
	normal[1] = Vector3D(0, 0, -1);
	normal[2] = Vector3D(0, 1, 0);
	normal[3] = Vector3D(0, -1, 0);
	normal[4] = Vector3D(1, 0, 0);
	normal[5] = Vector3D(-1, 0, 0);
	bool rayHit = false;
	
	double numer, denom;
	double t[6];
	Point3D pointOnPlane = m_pos;
	Point3D hitPos[6];
	
	// Intersect with front face
	pointOnPlane[2] += m_size;
	ray.origin[2] += epsilon;
	numer = normal[0].dot(pointOnPlane - ray.origin);
	denom = normal[0].dot(ray.dir);
	t[0] = numer/denom;
	
	hitPos[0] = ray.origin + (t[0] * ray.dir);
	
	// Intersect with the back face	
	pointOnPlane[2] -= m_size;
	ray.origin[2] -= epsilon;
	numer = normal[1].dot(pointOnPlane - ray.origin);
	denom = normal[1].dot(ray.dir);
	t[1] = numer/denom;
	hitPos[1] = ray.origin + (t[1] * ray.dir);

	// Intersect with top face
	pointOnPlane = m_pos;
	pointOnPlane[1] += m_size;
	ray.origin[1] += epsilon;
	numer = normal[2].dot(pointOnPlane - ray.origin);
	denom = normal[2].dot(ray.dir);
	t[2] = numer/denom;	
	hitPos[2] = ray.origin + (t[2] * ray.dir);
	
	// Intersect with Botom face
	pointOnPlane[1] -= m_size;
	ray.origin[1] -= epsilon;
	numer = normal[3].dot(pointOnPlane - ray.origin);
	denom = normal[3].dot(ray.dir);
	t[3] = numer/denom;	
	hitPos[3] = ray.origin + (t[3] * ray.dir);
		
	// Intersect with side face
	pointOnPlane[0] += m_size;
	ray.origin[0] += epsilon;
	numer = normal[4].dot(pointOnPlane - ray.origin);
	denom = normal[4].dot(ray.dir);
	t[4] = numer/denom;
	hitPos[4] = ray.origin + (t[4] * ray.dir);
	
	// Intersect with other side face		
	pointOnPlane[0] -= m_size;
	ray.origin[0] -= epsilon;
	numer = normal[5].dot(pointOnPlane - ray.origin);
	denom = normal[5].dot(ray.dir);
	t[5] = numer/denom;
	hitPos[5] = ray.origin + (t[5] * ray.dir);
	
	double tMin = LONG_MAX;
	Point3D actualHitPos;
	int index = 0;
	for (int i = 0;i<6;i++)
	{
		if (tMin > t[i] && normal[i].dot(ray.dir) > epsilon && t[i] > epsilon)
		{
			tMin = t[i];
			actualHitPos = hitPos[i];
			index = i;
		}
	}
	
	if (actualHitPos[0] >= m_pos[0] && actualHitPos[1] >= m_pos[1] && actualHitPos[2] >= m_pos[2] && 
		actualHitPos[0] <= m_pos[0] + m_size && actualHitPos[1] <= m_pos[1] + m_size && actualHitPos[2] <= m_pos[2] + m_size)
	{		
		
		if (ray.isHit() && tMin > ray.t)
		{
			// This object is closer to the origin so we will update our ray
			return false;
		}			
						
		ray.n = normal[index];
		ray.t = tMin;			
		ray.setHit(true);
		return true;
	}*/
	
	double ox = ray.origin[0]; 
	double oy = ray.origin[1]; 
	double oz = ray.origin[2];
	double dx = ray.dir[0]; 
	double dy = ray.dir[1]; 
	double dz = ray.dir[2];

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max; 

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (m_pos[0] - ox) * a;
		tx_max = (m_pos[0] + m_size - ox) * a;
	}
	else {
		tx_min = (m_pos[0] + m_size - ox) * a;
		tx_max = (m_pos[0] - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (m_pos[1] - oy) * b;
		ty_max = (m_pos[1] + m_size - oy) * b;
	}
	else {
		ty_min = (m_pos[1] + m_size - oy) * b;
		ty_max = (m_pos[1] - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (m_pos[2] - oz) * c;
		tz_max = (m_pos[2] + m_size - oz) * c;
	}
	else {
		tz_min = (m_pos[2] + m_size - oz) * c;
		tz_max = (m_pos[2] - oz) * c;
	}
	
	double t0, t1;
	int face_in, face_out;
	double tmin = LONG_MAX;
	// find largest entering t value

	if (tx_min > ty_min) {
		t0 = tx_min;
		face_in = (a >= 0.0) ? 0 : 3;
	}
	else {
		t0 = ty_min;
		face_in = (b >= 0.0) ? 1 : 4;
	}

	if (tz_min > t0) {
		t0 = tz_min;
		face_in = (c >= 0.0) ? 2 : 5;
	}

	// find smallest exiting t value

	if (tx_max < ty_max) {
		t1 = tx_max;
		face_out = (a >= 0.0) ? 3 : 0;
	}
	else {
		t1 = ty_max;
		face_out = (b >= 0.0) ? 4 : 1;
	}

	if (tz_max < t1) {
		t1 = tz_max;
		face_out = (c >= 0.0) ? 5 : 2;
	}

	if (t0 < t1 && t1 > epsilon) {  // condition for a hit
		if (t0 > epsilon) {
			tmin = t0;  			// ray hits outside surface

			ray.n = get_normal_for_cube(face_in);
		}
		else {
			tmin = t1;				// ray hits inside surface

			ray.n = get_normal_for_cube(face_out);
		}
		
		if (tmin == LONG_MAX && !ray.isHit())
		{
			tmin = 0;
			return false;
		}
		
		if (ray.isHit() && tmin > ray.t)
			return false;
		
		ray.t = tmin;
//		ray.n = get_normal(face_out);
		ray.setHit(true);
		return (true);
	}
	
	return false;
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

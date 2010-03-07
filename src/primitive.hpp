#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"
#include "ray.hpp"


class Primitive {
public:
	virtual ~Primitive();

	virtual bool hit (Ray& ray, double episilon) = 0;
};

class Sphere : public Primitive {
public:
	virtual ~Sphere();
	virtual bool hit (Ray& ray, double episilon);
};

class Cube : public Primitive {
public:
	virtual ~Cube();

	virtual bool hit (Ray& ray, double episilon);
};

class NonhierSphere : public Primitive {
public:
	NonhierSphere(const Point3D& pos, double radius)
		: m_pos(pos), m_radius(radius)
	{
	}
	virtual ~NonhierSphere();
	
	virtual bool hit (Ray& ray, double episilon);
private:
  Point3D m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
	NonhierBox(const Point3D& pos, double size)
		: m_pos(pos), m_size(size)
	{
	}

	virtual ~NonhierBox();
	virtual bool hit (Ray& ray, double episilon);
	
	Vector3D get_normal(int face)
	{
		switch (face)
		{
			case 0:
				return Vector3D(-1, 0, 0);
			case 1:
				return Vector3D(0, -1, 0);
			case 2:
				return Vector3D(0, 0, -1);
			case 3:
				return Vector3D(1, 0, 0);
			case 4:
				return Vector3D(0, 1, 0);
			case 5:
				return Vector3D(0, 0, 1);
		}
	}
private:
	Point3D m_pos;
	double m_size;
};

class Plane : public Primitive 
{
	public:
//		Plane(Vector3D u, Vector3D v);
		Plane(Point3D pointOnPlane, Vector3D n);
//		Plane();
		virtual ~Plane();
		virtual bool hit (Ray& ray, double episilon);
	
	private:
		// Define plane using two vectors
//		Vector3D u, v;
		
		// Define plane using a point and a normal
		Point3D pos;
		Vector3D normal;
	
};

#endif

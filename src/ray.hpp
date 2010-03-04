#ifndef CS488_RAY_HPP
#define CS488_RAY_HPP

#include "algebra.hpp"
#include "material.hpp"

// Represents a ray
class Ray {
	public:
		Point3D origin;
		Vector3D dir;
		
		// Properties for after a hit
		Material *material;
		double t; // Parameter of how far along ray we are before we hit an object
		Vector3D n; // normal
		std::string name;
		
		bool secondaryRay;
		
		
		Ray();
		Ray(const Point3D& origin, const Vector3D& dir);
		Ray(const Ray& ray);
		
		Ray& operator =(const Ray& other)
		{
			origin = other.origin;
			dir = other.dir;
			
			n = other.n;
			name = other.name;
			t = other.t;
			material = other.material;
			hit = other.isHit();
			return *this;
		}
		
		~Ray(void);
		
		bool isHit() const
		{
			return hit;
		}
		
		void setHit(bool hitVal)
		{
			hit = hitVal;
		}
		
		Point3D getHitPos()
		{
			return origin + (t * dir);
		}
		
	private:
		bool hit;
};


#endif

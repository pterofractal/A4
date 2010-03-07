#ifndef CS488_MESH_HPP
#define CS488_MESH_HPP

#include <vector>
#include <iosfwd>
#include "primitive.hpp"
#include "algebra.hpp"
#include "ray.hpp"

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh(const std::vector<Point3D>& verts,
       const std::vector< std::vector<int> >& faces);

		virtual bool hit (Ray& ray, double episilon);
  typedef std::vector<int> Face;
  
	bool sameSide(Point3D p1, Point3D p2, Point3D a, Point3D b)
	{
		Vector3D temp1 = (b - a).cross(p1 - a);
		Vector3D temp2 = (b - a).cross(p2 - a);

		if (temp1.dot(temp2) >= 0)
		{
			return true;
		}
			
		
		return false;
	}
	
	bool pointInTriangle (Point3D p, Point3D a, Point3D b, Point3D c)
	{
		if (sameSide(p, a, b, c) && sameSide(p, b, a, c) && sameSide(p, c, a, b))
		{
			return true;
		}
				
		return false;			
	}
	
private:
  std::vector<Point3D> m_verts;
  std::vector<Face> m_faces;

	Point3D mid;
	double sphereRad;

  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};

#endif

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
 	
private:
  std::vector<Point3D> m_verts;
  std::vector<Face> m_faces;

	Point3D mid;
	double sphereRad;

  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};

#endif

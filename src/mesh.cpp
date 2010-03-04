#include "mesh.hpp"
#include "algebra.hpp"
#include <iostream>

Mesh::Mesh(const std::vector<Point3D>& verts,
           const std::vector< std::vector<int> >& faces)
  : m_verts(verts),
    m_faces(faces)
{
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  std::cerr << "mesh({";
  for (std::vector<Point3D>::const_iterator I = mesh.m_verts.begin(); I != mesh.m_verts.end(); ++I) {
    if (I != mesh.m_verts.begin()) std::cerr << ",\n      ";
    std::cerr << *I;
  }
  std::cerr << "},\n\n     {";
  
  for (std::vector<Mesh::Face>::const_iterator I = mesh.m_faces.begin(); I != mesh.m_faces.end(); ++I) {
    if (I != mesh.m_faces.begin()) std::cerr << ",\n      ";
    std::cerr << "[";
    for (Mesh::Face::const_iterator J = I->begin(); J != I->end(); ++J) {
      if (J != I->begin()) std::cerr << ", ";
      std::cerr << *J;
    }
    std::cerr << "]";
  }
  std::cerr << "});" << std::endl;
  return out;
}

bool Mesh::hit (Ray& ray, double epsilon)
{

	Ray duplicateRay = ray;
	Ray backUp = duplicateRay;
	int index = -1;
	int backupI = index;
	bool validHit = false;
	for (int i = 0;i<m_faces.size();i++)
	{
		if (m_faces[i].size() >= 3)
		{
			Vector3D a = m_verts[m_faces[i][1]] - m_verts[m_faces[i][0]];	
			Vector3D b = m_verts[m_faces[i][2]] - m_verts[m_faces[i][0]];
			Vector3D n = a.cross(b);
			n.normalize();
			Plane p(m_verts[m_faces[i][0]], n);
			if (p.hit(duplicateRay, epsilon))
			{			
				if (!pointInTriangle(duplicateRay.getHitPos(), m_verts[m_faces[i][0]], m_verts[m_faces[i][1]], m_verts[m_faces[i][2]]))
				{
					duplicateRay = backUp;
					//index = backupI;
				}			
				else
				{
					index = i;
					validHit = true;
					backUp = duplicateRay;
					backupI = index;
				}		
			}
		}
	}
	
	if (index != -1)
	{
/*		std::cout << "Ray\t" << duplicateRay.getHitPos() << "t\t" << duplicateRay.t << "\n";*/
		ray = duplicateRay;
		
		/*ray.t = duplicateRay.t;
		ray.n = duplicateRay.n;
		ray.setHit(true);*/
		return true;
	}
		
	
	return false;
}
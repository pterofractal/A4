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
	// duplicateRay is simply used as a backup so that we don't destroy valid
	// values inside ray
	Ray duplicateRay = ray;
	
	// backUp is used to restore duplicateRay to last valid ray intersection with a 
	// plane of the mesh
	Ray backUp = duplicateRay;
	
	// index is used to determine which plane was hit
	int index = -1;
	
	// Iterate through each face
	for (int i = 0;i<m_faces.size();i++)
	{
		// We need at least 3 points to define a plane
		if (m_faces[i].size() >= 3)
		{
			// Assume the points are in general position 
			// thus we only need the first 3 points to form a plan
			Vector3D a = m_verts[m_faces[i][1]] - m_verts[m_faces[i][0]];	
			Vector3D b = m_verts[m_faces[i][2]] - m_verts[m_faces[i][0]];
			
			// Construct a normal to the plane
			Vector3D n = a.cross(b);
			n.normalize();
			
			// Create a plane object
			Plane p(m_verts[m_faces[i][0]], n);
			
			// Intersect the ray with the plane
			if (p.hit(duplicateRay, epsilon))
			{			
				bool pointInPlane = false;
				
				// Divide plane into triangles and check each triangle for the point
				for (int j = 1;j<m_faces[i].size() - 1;j++)
				{
					if (pointInTriangle(duplicateRay.getHitPos(), m_verts[m_faces[i][0]], m_verts[m_faces[i][j]], m_verts[m_faces[i][j+1]]))
					{
						pointInPlane = true;
						break;
					}
				}
				
				// If the point isn't in the plane we will restore our ray back to the original
				if (!pointInPlane)
				{
					duplicateRay = backUp;
				}			
				else
				{
					// If the ray actual hit something in the plane we will store its index
					// and all other values into the backup.
					index = i;
					backUp = duplicateRay;
				}		
			}
		}
	}
	
	if (index != -1)
	{
		// We have a valid hit
		ray = duplicateRay;		
		return true;
	}	
	
	return false;
}
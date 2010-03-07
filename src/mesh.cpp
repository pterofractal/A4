#include "mesh.hpp"
#include "algebra.hpp"
#include <iostream>

Mesh::Mesh(const std::vector<Point3D>& verts,
           const std::vector< std::vector<int> >& faces)
  : m_verts(verts),
    m_faces(faces)
{
	Point3D min, max;
	min = m_verts[0];
	max = m_verts[0];
	for (int i = 1;i<m_verts.size()-1;i++)
	{
		if (min[0] > m_verts[i][0])
			min[0] = m_verts[i][0];
			
		if (min[1] > m_verts[i][1])
			min[1] = m_verts[i][1];
			
		if (min[2] > m_verts[i][2])
			min[2] = m_verts[i][2];
		
		if (max[0] < m_verts[i][0])
			max[0] = m_verts[i][0];
		
		if (max[1] < m_verts[i][1])
			max[1] = m_verts[i][1];
			
		if (max[2] < m_verts[i][2])
			max[2] = m_verts[i][2];			
	}

	mid = Point3D(min[0] + max[0], min[1] + max[1], min[2] + max[2]);
	mid[0] /= 2.0;
	mid[1] /= 2.0;
	mid[2] /= 2.0;
	sphereRad = (mid - min).length();
		
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
	// Bounding sphere
	NonhierSphere boundingSphere(mid, sphereRad);
	
	// duplicateRay is simply used as a backup so that we don't destroy valid
	// values inside ray
	Ray duplicateRay = ray;
	
	Ray backUp = ray;
	
	// Check if ray hits bounding sphere
	if (!boundingSphere.hit(duplicateRay, epsilon))
		return false;
	
	duplicateRay = ray;
	// index is used to determine which plane was hit
	int index = -1;

	// Matrices and vectors for Cramers rule
	Matrix4x4 D1, D2, D3, D;
	Vector3D vec1, vec2, vec3, vec4;
	bool ret = false;

	// Column 3 from D
	vec3 = ray.dir;
	for (unsigned int i = 0;i<m_faces.size();i++)
	{
		
		// The RHS of the equation
		vec4 = ray.origin - m_verts[m_faces[i][0]];
		for (unsigned int j = 1;j<m_faces[i].size() - 1;j++)
		{
			// Column 1 of D
			vec1 = m_verts[m_faces[i][j]] - m_verts[m_faces[i][0]];
			vec2 = m_verts[m_faces[i][j+1]] - m_verts[m_faces[i][0]];
			
			for (int k = 0;k<3;k++)
			{
				D[k][0] = vec1[k];
				D[k][1] = vec2[k];
				D[k][2] = vec3[k];
				  
				D1[k][0] = vec4[k];
				D1[k][1] = vec2[k];
				D1[k][2] = vec3[k];
				               
				D2[k][0] = vec1[k];
				D2[k][1] = vec4[k];
				D2[k][2] = vec3[k];
				               
				D3[k][0] = vec1[k];
				D3[k][1] = vec2[k];
				D3[k][2] = vec4[k];
			}	
			
			double beta, gamma, t;
			double dDet = 1 / D.det(D);
			beta = D1.det(D1) * dDet;
			gamma = D2.det(D2) * dDet;
			t = D3.det(D3) * dDet;
			t = -1 * t;
			
			if (beta >= epsilon && gamma >= epsilon && beta + gamma <= 1 && t > epsilon)
			{
				if (ray.isHit() && ray.t < t)
					continue;
				
				// Get two points on the triangle
				Vector3D a = m_verts[m_faces[i][1]] - m_verts[m_faces[i][0]];	
				Vector3D b = m_verts[m_faces[i][2]] - m_verts[m_faces[i][0]];
				
				// Compute norm of triangle
				Vector3D n = a.cross(b);
				n.normalize();
				
				// Update ray
				ray.t = t;
				// std::cout << "t\t" << t << std::endl;
				ray.n = n;
				ray.setHit(true);				
				ray.hitPos = ray.getHitPos();
				ret = true;
			}	
		}
	}
	
	return ret;
	
	// Iterate through each face
	for (unsigned int i = 0;i<m_faces.size();i++)
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
				for (unsigned int j = 1;j<m_faces[i].size() - 1;j++)
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
		std::cout << ray.getHitPos() << "\n";
		return true;
	}	
	
	return false;
}
#include "a4.hpp"
#include "image.hpp"
#include "algebra.hpp"
#include <math.h>

void a4_render(// What to render
               SceneNode* root,
               // Where to output the image
               const std::string& filename,
               // Image size
               int width, int height,
               // Viewing parameters
               const Point3D& eye, const Vector3D& view,
               const Vector3D& up, double fov,
               // Lighting parameters
               const Colour& ambient,
               const std::list<Light*>& lights
               )
{
  // Fill in raytracing code here.

  std::cerr << "Stub: a4_render(" << root << ",\nFile name:\t     "
            << filename << ", " << width << ", " << height << ",\nEye:\t     "
            << eye << ", " << view << ", " << up << ", " << fov << ",\nAmbient:\t     "
            << ambient << ",\n     {";

  for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
    if (I != lights.begin()) std::cerr << ", ";
    std::cerr << **I;
  }
  std::cerr << "});" << std::endl;
  
	double L_in[lights.size()];
	int index = 0;
	Image img(width, height, 3);
	double aspectRatio = width/height;
	Matrix4x4  T, R, S, M;
	Point3D pixelWorld;
	
	double d = 5;
	
	// Move Matrix
	M[0][3] = -1 * width / 2.0;
	M[1][3] = -1 * height / 2.0;
	M[2][3] = d;
	
	// Scale Matrix
	double h = 2 * d * tan (fov / 2);
	S[0][0] = h / height;
	S[1][1] = h / height;
	
	
	// Rotate matrix to super impose WCS to VCS
	Vector3D w = Point3D(view[0], view[1], view[2]) - eye;
	w.normalize();
	
	Vector3D u = up.cross(w);
	u.normalize();
	
	Vector3D v = w.cross(u);
	v.normalize();
	
	for (int i = 0;i<3;i++)
	{
		R[i][0] = u[i];
		R[i][1] = v[i];
		R[i][2] = w[i];
	}
	
	// Final transform matrix
	T[0][3] = eye[0];
	T[1][3] = eye[1];
	T[2][3] = eye[2];
	
	
	Ray *ray = new Ray();
	//ray->dir = view;
	ray->origin = eye;
	for (int y = 0;y<height;y++)
	{
		for (int x = 0;x<width;x++)
		{
			pixelWorld[0] = x;
			pixelWorld[1] = y;
			pixelWorld[2] = 0;
			
			pixelWorld = M * pixelWorld;
			pixelWorld = S * pixelWorld;
			pixelWorld = R * pixelWorld;
			pixelWorld = T * pixelWorld;
			
			//ray->origin = origin;
			ray->dir = pixelWorld - eye;
			//std::cerr << "pixelWorld\t" << pixelWorld << "\tray dir " << ray->dir << "\tx " << x << "\t" << y << "\n";
			root->hit(ray, 0.01);
			
			if (ray->isHit())
			{
				Colour roh(0,0,0);
				for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) 
				{
					Light *l = *I;
					Vector3D temp = pixelWorld - l->position;
					temp.normalize();
	 				roh = ( (PhongMaterial *)ray->material)->get_kd();
					roh = (temp.dot(ray->n) ) * roh;
					roh = roh * l->colour;
				}

				std::cerr << "roh \t" << roh << "\n";
				
				// Red: increasing from top to bottom
				img(x, y, 0) = roh.R();
				// Green: increasing from left to right
				img(x, y, 1) = roh.G();
				// Blue: in lowpper-right corners
				img(x, y, 2) = roh.B();
				
			}
			else
			{
				img(x, y, 0) = 0;
				img(x, y, 1) = 0;
				img(x, y, 2) = 0;
			}
			ray->setHit(false);
		}	
	}

  img.savePng(filename);
  
}
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

  std::cout << "Stub: a4_render(" << root << ",\nFile name:\t     "
            << filename << ", " << width << ", " << height << ",\nEye:\t     "
            << eye << ", " << view << ", " << up << ", " << fov << ",\nAmbient:\t     "
            << ambient << ",\n     {";

  for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
    if (I != lights.begin()) std::cout << ", ";
    std::cout << **I;
  }
  std::cout << "});" << std::endl;
  
	double L_in[lights.size()];
	int index = 0;
	Image img(width, height, 3);
	double aspectRatio = width/height;
	Matrix4x4  T, R, S, M;
	Point3D pixelWorld;
	
	double d = 100;
	
	// Move Matrix
	M[0][3] = -1 * width / 2.0;
	M[1][3] = -1 * height / 2.0;
	M[2][3] = d;
	
	// Scale Matrix
	double h = 3 * d * tan (fov);
	S[0][0] = h / height;
	S[1][1] = h / height;
	
	
	// Rotate matrix to super impose WCS to VCS
	Vector3D w = Point3D(view[0], view[1], view[2]) - eye;
	w.normalize();
	
	Vector3D u = up.cross(w);
	u.normalize();
	
	Vector3D v = w.cross(u);
	//v.normalize();
	
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
			// Start with screen position
			pixelWorld[0] = x;
			pixelWorld[1] = y;
			pixelWorld[2] = 0;
			
			// Push screen pixel through each transformation			
			pixelWorld = M * pixelWorld;
			pixelWorld = S * pixelWorld;
			pixelWorld = R * pixelWorld;
			pixelWorld = T * pixelWorld;
			
			// Determine the direction of the ray
			ray->dir = pixelWorld - eye;
			ray->dir.normalize();
			
			// Intersect the ray with all the objects
			root->hit(ray, 0.0000000001);
			
			if (ray->isHit())
			{
				// Colour variable that will be used to determine colour for a particular pixel
				Colour roh(0,0,0);
				
				// Iterate through each light source
				for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) 
				{
					// Get light pointer
					Light *l = *I;

					// Colour values for diffuse and specular
					Colour diffuse, specular;

					// Determine the direction of light
					Vector3D directionOfLight = l->position - ray->getHitPos();
					directionOfLight.normalize();
					
					// Construct a ray representing the light
					Ray *lightRay = new Ray(ray->getHitPos(), directionOfLight);
					lightRay->setHit(false);
					lightRay->secondaryRay = true;
					root->hit(lightRay, 0.0000000001);
//					std::cout<<"ray dir\t" << lightRay->t << "\n";
					if (!lightRay->isHit())
					{
						// If the light ray doesn't hit anything then the light from 
						// the light source reaches the hit point

						// Compute the reflection vector
						Vector3D r = 2 * (directionOfLight.dot(ray->n))*ray->n - directionOfLight;
						r.normalize();
						
						// Calculate the diffuse value
						diffuse = ( (PhongMaterial *)ray->material)->get_kd();
						diffuse = (directionOfLight.dot(ray->n)) * diffuse;
						
						// Calculate the specular value
						specular = ( (PhongMaterial *)ray->material)->get_ks();
						specular = pow(r.dot(ray->dir), ( (PhongMaterial *)ray->material)->get_shininess()) * specular;

						// Need to make sure the specular value isn't negative
						if (r.dot(ray->dir) < 0)
							specular = -1 * specular;

						// Calculate how far the light is from the hit position
						double dist = directionOfLight.length();
						
						// Calculate the attenuation
						double atten = 1 / (l->falloff[2] + l->falloff[1] * dist + l->falloff[0] * dist * dist);
						
						// Bring values together
						roh = roh + specular + diffuse * l->colour;
						roh = roh * atten;
			//									std::cout << "roh\t" << roh << "\n";
					}
				}
				
				// Add the ambient to our colour values
				roh = roh + ambient;
				
				//std::cout << "roh \t" << roh << "\n";
				// Red: increasing from top to bottom
				img(x, y, 0) = roh.R();
				// Green: increasing from left to right
				img(x, y, 1) = roh.G();
				// Blue: in lowpper-right corners
				img(x, y, 2) = roh.B();
				
			}
			else
			{
				// Create blue gradient by default if ray misses
				double gradient = (1.0 * y)/height;
				img(x, y, 0) = 0;
				img(x, y, 1) = 0;
				img(x, y, 2) = gradient * gradient;
			}
			ray->setHit(false);
		}	
	}

  img.savePng("z" + filename);
  
}
#include "a4.hpp"
#include "image.hpp"
#include "algebra.hpp"

#include <math.h>

Matrix4x4 transformationMatrix;
pthread_mutex_t mutex1;
int samples = 1;
int numThreads = 1;

void testtest(Ray* ray)
{
	pthread_mutex_lock(&mutex1);
	//if (ray.dir.length() < 1)
	//	std::cerr << "err dir\t" << ray.dir << " length " << ray.dir.length() << std::endl;
	if (ray->n.length() < 1)
		std::cerr << "err\t" << ray->n << " length " << ray->n.length() << std::endl;
	pthread_mutex_unlock(&mutex1);
}

void a4_render(	// What to render
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
		std::list<Light*>& lights
               )
{
  // Fill in raytracing code here.
	pthread_mutex_init(&mutex1, NULL);
	std::cout << "Stub: a4_render(" << root << ",\nFile name:\t     "
		        << filename << ", " << width << ", " << height << ",\nEye:\t     "
		        << eye << ", " << view << ", " << up << ", " << fov << ",\nAmbient:\t     "
		        << ambient << ",\n     {";

	for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) 
	{
		if (I != lights.begin()) std::cout << ", ";
			std::cout << **I;
	}
	std::cout << "});" << std::endl;
  
	Image img(width, height, 3);
	Matrix4x4  T, R, S, M;
	Point3D pixelWorld;
	
	double d = 100;
	
	// Move Matrix
	M[0][3] = -1 * width / 2.0;
	M[1][3] = -1 * height / 2.0;
	M[2][3] = d;
	
	// Scale Matrix
	fov = -1 * fov * M_PI / 180;
	double h = 2 * d * tan (fov / 2.0);
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
	
	transformationMatrix = T * R * S * M;

	TraceArgs traceArgs[numThreads];
	pthread_t threads[numThreads]; 
	
	Image background(400, 400, 3);
	background.loadPng("background.png");

	for (int i = 0;i<numThreads;i++)
	{
		traceArgs[i].background = background;
		traceArgs[i].img = &img;
		traceArgs[i].lights = &lights;
		traceArgs[i].eye = eye;
		traceArgs[i].root = root;
		traceArgs[i].ambient = ambient;		
		traceArgs[i].height = height;
	}

	//Seed random number generator
	srand( (unsigned)time( 0 ) );

	int errcode;
	double sections = 1.0/numThreads;
	for (int i = 0;i<numThreads;i++)
	{
		traceArgs[i].xMin = 0;
		traceArgs[i].xMax = width;
		traceArgs[i].yMin = (i) * sections * height;
		traceArgs[i].yMax = (i+1) * sections * height;
		//std::cout << traceArgs[i].xMin << "\t" << traceArgs[i].xMax << "\t" << traceArgs[i].yMin << "\t" << traceArgs[i].yMax << std::endl;

		if (errcode=pthread_create(&threads[i],					 
									NULL,						 
									ray_trace,          	     
									&traceArgs[i])) 			 
		{					         
			std::cerr << errcode << "pthread_create\n";
		}
	}

	for (int i = 0;i<numThreads;i++)
	{
		// wait for thread to terminate 
		if (errcode=pthread_join(threads[i],NULL))
		{ 
			std::cerr << errcode << "join\n";
		}
		else 
		{
			std::cout << "Thread " << i << " complete\n";
		}
	}

  img.savePng("z" + filename);
  
}



void *ray_trace(void *arg)
{
	TraceArgs traceArgs = *(TraceArgs *)arg;
	std::list<Light*> lights =	*traceArgs.lights;
	Ray *ray = new Ray();
	ray->origin = traceArgs.eye;
	Point3D pixelWorld;
	double samplesD = samples;
	for (int y = traceArgs.yMin;y<traceArgs.yMax;y++)
	{
		for (int x = traceArgs.xMin;x<traceArgs.xMax;x++)
		{
			// Colour variable that will be used to determine colour for a particular pixel
			Colour roh[samples][samples];
			bool hitSomething = false;

			for (int j = 0;j<samples;j++)
			{
				for (int k = 0;k<samples;k++)
				{					
					// Start with screen position
					if (samples != 1)
					{
						float r = (float)rand() / (float)RAND_MAX;
						// If we are taking more than 1 sample randomly add a small amount to our pixel
						pixelWorld[0] = x + (k + r)/samplesD;
						pixelWorld[1] = y + (j + r)/samplesD;
					}
					else
					{
						pixelWorld[0] = x;
						pixelWorld[1] = y;
					}
					pixelWorld[2] = 0;						
					
					
				
					// Push screen pixel through each transformation			
					pixelWorld = transformationMatrix * pixelWorld;

					// Determine the direction of the ray
					ray->dir = pixelWorld - traceArgs.eye;
					ray->dir.normalize();
					
					// Intersect the ray with all the objects
					traceArgs.root->hit(ray, 0.0000000001);
					Colour bgColour(traceArgs.background(x%300, y%300, 0), traceArgs.background(x%300, y%300, 1), traceArgs.background(x%300, y%300, 2));
					roh[j][k] = bgColour;
					
					if (ray->isHit())
					{
						// Add the ambient component
						roh[j][k] = ( (PhongMaterial *)ray->material)->get_kd() * traceArgs.ambient;
						
						//ray->n.normalize();
						//ray->dir.normalize();
						// Iterate through each light source
						for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) 
						{
							// Make a note that we hit something
							hitSomething= true;
							
							// Get light pointer
							Light *l = *I;
					
							// Colour values for diffuse and specular
							Colour diffuse, specular;

							// Determine the direction of light
							Vector3D directionOfLight = l->position - ray->getHitPos();

							// Calculate how far the light is from the hit position
							double dist = directionOfLight.length();

							// Normalize direction of light
							directionOfLight.normalize();

							// Construct a ray representing the light
							Ray *lightRay = new Ray(ray->getHitPos(), directionOfLight);
							lightRay->setHit(false);
							lightRay->secondaryRay = true;
							traceArgs.root->hit(lightRay, 0.0000000001);
							
							if (!lightRay->isHit() || (lightRay->isHit() && dist < (lightRay->getHitPos() - ray->getHitPos()).length() ) ) 
							{
								if (ray->n.dot(directionOfLight) < 0)
								{
									ray->n = -1 * ray->n;
								}
								
								// If the light ray doesn't hit anything then the light from 
								// the light source reaches the hit point
								
								// Compute the reflection vector
								Vector3D r = 2 * (directionOfLight.dot(ray->n))*ray->n - directionOfLight;
								r.normalize();

								// Calculate the diffuse value
								diffuse = ( (PhongMaterial *)ray->material)->get_kd();
								diffuse = (directionOfLight.dot(ray->n)) * diffuse ;				
								
								// Calculate the specular value
								specular = ( (PhongMaterial *)ray->material)->get_ks();
								specular = pow(r.dot(ray->dir), ( (PhongMaterial *)ray->material)->get_shininess()) * specular;

								// Need to make sure the specular value isn't negative								
								if (specular.R() < 0 && specular.G() < 0 && specular.B() < 0)
								{
									specular = -1 * specular;
								}
								// Calculate the attenuation
								double atten = 1 / (l->falloff[0] + l->falloff[1] * dist + l->falloff[2] * dist * dist);

								// Bring values together
								roh[j][k] = roh[j][k] + specular + diffuse * l->colour * atten;								
							}
							else
							{
								//Light is being blocked
							}
						}
					}
					ray->setHit(false);
				}
			}

				Colour averageRoh;
				//int successfulSamples = 0;
				for (int j = 0;j<samples;j++)
				{
					for (int k=0;k<samples;k++)
					{
						averageRoh = averageRoh + roh[j][k];	
					}
				}

				if (!hitSomething)
				{
					// Create blue gradient by default if ray misses
					double gradient = (1.0 * y)/traceArgs.height;
					(*traceArgs.img)(x, y, 0) = traceArgs.background(x%300, y%300, 0);
					(*traceArgs.img)(x, y, 1) = traceArgs.background(x%300, y%300, 1);
					(*traceArgs.img)(x, y, 2) = traceArgs.background(x%300, y%300, 2);
				}				
				else
				{
					if (samples != 1)
						averageRoh = (1.0 / (samples * samples) ) * averageRoh;
					
					// Red: increasing from top to bottom
					(*traceArgs.img)(x, y, 0) = averageRoh.R();

					// Green: increasing from left to right
					(*traceArgs.img)(x, y, 1) = averageRoh.G();

					// Blue: in lowpper-right corners
					(*traceArgs.img)(x, y, 2) = averageRoh.B();
				}			
				hitSomething = false;
		}	
	}
	delete(ray);
	return arg;
}
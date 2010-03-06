#ifndef CS488_A4_HPP
#define CS488_A4_HPP

#include <string>
#include "algebra.hpp"
#include "scene.hpp"
#include "light.hpp"
#include "ray.hpp"
#include "image.hpp"

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
               std::list<Light*>& lights
               );

struct TraceArgs {
	Image* img;
	std::list<Light*>* lights;
	Point3D eye;
	SceneNode* root;
	Colour ambient;
	int xMin, xMax, yMin, yMax, height;	
};

void *ray_trace(void *arg);
#endif

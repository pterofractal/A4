#ifndef CS488_UTIL_HPP
#define CS488_UTIL_HPP

#include "algebra.hpp"
#include "material.hpp"

// Class with simple functions


Vector3D get_normal_for_cube(int face)
{
	switch (face)
	{
		case 0:
			return Vector3D(-1, 0, 0);
		case 1:
			return Vector3D(0, -1, 0);
		case 2:
			return Vector3D(0, 0, -1);
		case 3:
			return Vector3D(1, 0, 0);
		case 4:
			return Vector3D(0, 1, 0);
		case 5:
			return Vector3D(0, 0, 1);
		default:
			return Vector3D(1, 0, 0);
	}
}


double max (double a, double b)
{
	if (a > b)
		return a;
	return b;
}

double min (double a, double b)
{
	if (a < b)
		return a;
	return b;
}

#endif

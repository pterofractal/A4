#ifndef CS488_UTIL_HPP
#define CS488_UTIL_HPP

#include "algebra.hpp"
#include "material.hpp"

// Class with simple functions

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

#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

#include "GLTexture.hpp"

struct Color 
{
	Color() : r(1), g(1), b(1), a(1) {}
	Color(int r, int g, int b, int a):
	r(r), g(g), b(b), a(a) {}

	unsigned char r, g, b, a;
};

class Material
{
public:

	Color tint;
	GLTexture* texture;

	void dispose() { delete texture; }

};

#endif
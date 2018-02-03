#ifndef _PRIMITIVES_HPP_
#define _PRIMITIVES_HPP_

#include "graphics/Model.hpp"

class Primitives 
{
public:

    static Model* create_cube();
    // Effect: Creates a new cube primitive and returns a pointer to it

    static Model* create_quad(const float w, const float h);
	// Effect: Creates a quad and returns a pointer to the model

};


#endif
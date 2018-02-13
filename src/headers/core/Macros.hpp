#ifndef _MACROS_HPP_
#define _MACROS_HPP_
#include <iostream>
#include "Errors.hpp"
#include <GL\glew.h>
#include <assert.h>

#define cout(x) std::cout << x;
#define coutln(x) std::cout << x << std::endl;
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define CHECK_GL_ERROR() { GLenum err; while( (err = glGetError()) != GL_NO_ERROR) { std::cout << "OpenGL error: " << err << std::endl; } }


#endif
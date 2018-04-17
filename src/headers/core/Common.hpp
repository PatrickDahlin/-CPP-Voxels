#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <cstdint>
#include <assert.h>
#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>

typedef std::int64_t		s64;
typedef std::uint64_t		u64;
typedef std::int32_t 		s32;
typedef std::uint32_t 		u32;
typedef std::int16_t 		s16;
typedef std::uint16_t		u16;
typedef std::int8_t			byte;
typedef std::uint8_t		ubyte;

typedef double 				f64;
typedef float 				f32;

typedef glm::vec2			vec2;
typedef glm::vec3			vec3;
typedef glm::vec4			vec4;

typedef glm::mat3			mat3;
typedef glm::mat4			mat4;

#define my_assert(x,y) if(!(x)){std::cout << "\n\nAssertion failed!\n"<< __FILE__<<":"<<__LINE__<<"\n"<<y<<std::flush; exit(-1);}

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define CHECK_GL_ERROR() { GLenum err; while( (err = glGetError()) != GL_NO_ERROR) { std::cout << "OpenGL error: " << err << std::endl; } }



#endif
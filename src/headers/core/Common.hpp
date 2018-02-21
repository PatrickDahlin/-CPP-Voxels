#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <cstdint>
#include <assert.h>
	
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

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define CHECK_GL_ERROR() { GLenum err; while( (err = glGetError()) != GL_NO_ERROR) { std::cout << "OpenGL error: " << err << std::endl; } }


#endif
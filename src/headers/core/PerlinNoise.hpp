#ifndef _PERLINNOISE_HPP_
#define _PERLINNOISE_HPP_

//
// Perlin Noise implementation
// CPP based off of Improved Noise in Java by Ken Perlin
//

extern float fperlin_noise(float x, float y, float z);

extern float fperlin_fractal(float x, float y, float z, int octaves, float persistence);

#endif
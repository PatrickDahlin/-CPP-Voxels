#ifndef _GLTEXTURE_HPP_
#define _GLTEXTURE_HPP_

#include <GL/glew.h>


enum class ColorFormat { RGB, RGBA };

class GLTexture 
{
public:
    GLTexture(ColorFormat format, unsigned char* data, int width, int height);
    ~GLTexture();

    void bind();

    void dispose();

protected:

    void upload_data(unsigned char* data);

    int         width, height;
    GLuint      tex_id;

    ColorFormat color_format;

};

extern GLTexture* load_image(const char* path, ColorFormat format = ColorFormat::RGB);


#endif

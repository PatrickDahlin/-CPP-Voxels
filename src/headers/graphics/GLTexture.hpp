#ifndef _GLTEXTURE_HPP_
#define _GLTEXTURE_HPP_


#include <GL\glew.h>

class GLTexture 
{
public:
    GLTexture(unsigned char* data, int width, int height);
    ~GLTexture();

    void bind();

    void dispose();

protected:

    void upload_data(unsigned char* data);

    int         width, height;
    GLuint      tex_id;

};

#endif

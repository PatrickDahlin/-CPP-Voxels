#ifndef _GLTEXTURE_HPP_
#define _GLTEXTURE_HPP_

#include <GL/glew.h>
#include "core/Disposable.hpp"

enum class ColorFormat { RGB, RGBA };

class GLTexture : public Disposable
{
public:
    GLTexture(ColorFormat format, unsigned char* data, int width, int height);
    virtual ~GLTexture();

    void bind();

    void dispose() override;

protected:

    void upload_data(unsigned char* data);

    int         width, height;
    GLuint      tex_id;

    ColorFormat color_format;

};

extern GLTexture* load_image(const char* path, ColorFormat format = ColorFormat::RGB);


#endif

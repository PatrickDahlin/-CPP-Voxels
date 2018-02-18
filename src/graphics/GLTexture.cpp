#include "graphics/GLTexture.hpp"
#include "core/Macros.hpp"
#include <stb_image.h>

GLTexture* load_image(const char* path, ColorFormat format)
{
	int width, height, channelnr;
	width = height = 0;
	channelnr = 0;
	unsigned char* data = stbi_load(path, &width, &height, &channelnr, 0);
	GLTexture* myTex = new GLTexture(format, data, width, height);
	stbi_image_free(data);
	return myTex;
}

GLTexture::GLTexture(ColorFormat format, unsigned char* data, int width, int height) :
width(width),
height(height),
tex_id(0),
color_format(format)
{
    upload_data(data);
}

GLTexture::~GLTexture()
{}

void GLTexture::bind()
{
    if(!tex_id)
        cout("Texture bound location 0");

    glBindTexture(GL_TEXTURE_2D, tex_id);
}

ColorFormat GLTexture::get_format() const
{
	return color_format;
}

void GLTexture::dispose()
{
	glDeleteTextures(1, &tex_id);
}

void GLTexture::upload_data(unsigned char* data)
{
	if(tex_id != 0){
		cout("error tried uploading data to existing texture");
		return;
	}
    glGenTextures(1, &tex_id);
    bind();

    // set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(color_format == ColorFormat::RGBA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	
	glGenerateMipmap(GL_TEXTURE_2D);

}
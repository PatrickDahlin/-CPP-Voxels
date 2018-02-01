#include "../headers/core/GLTexture.hpp"
#include "../headers/core/Macros.hpp"


GLTexture::GLTexture(unsigned char* data, int width, int height) :
width(width),
height(height),
tex_id(0)
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

void GLTexture::dispose()
{
	glDeleteTexture(&tex_id);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

}
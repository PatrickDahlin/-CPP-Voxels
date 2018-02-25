#include "graphics/FrameBuffer.hpp"
#include "graphics/ShaderProgram.hpp"
#include "graphics/Model.hpp"
#include <string>

static ShaderProgram* fbo_shader;
static GLuint quad_vao = 0, vbo;
static bool initialized = false;


void cleanup_framebuffers()
{
	if(!initialized) return;
	fbo_shader->dispose();
	delete fbo_shader;
	glDeleteVertexArrays(1, &quad_vao);
	glDeleteBuffers(1, &vbo);
	initialized = false;
}

void render_framebuffer(Framebuffer* buf)
{

	if(!initialized){
		glGenVertexArrays(1, &quad_vao);
		glBindVertexArray(quad_vao);

		static const GLfloat g_quad_vertex_buffer_data[] = {
			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f, 
			1.0f,  1.0f, 1.0f, 1.0f
		};

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, 16, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, 16, BUFFER_OFFSET(8));

		std::string vert = "layout (location = 0) in vec2 aPos;\
layout (location = 1) in vec2 aTexCoords;\
out vec2 TexCoords;\
\
void main()\
{\
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\
    TexCoords = aTexCoords;\
}";

	std::string frag = "out vec4 FragColor;\
in vec2 TexCoords;\
uniform sampler2D screenTexture;\
void main()\
{\
	float val = texture(screenTexture, TexCoords).r;\
    FragColor = vec4(texture(screenTexture, TexCoords).rgb,1.0);\
}";

		fbo_shader = new ShaderProgram(vert.c_str(),frag.c_str(),"#version 330 core\n");
		glBindVertexArray(0);

		initialized = true;
	}

	glClear(GL_COLOR_BUFFER_BIT);

	fbo_shader->use();

	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(quad_vao);
	buf->bind_color_buffer();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}


Framebuffer::Framebuffer(int width, int height) :
fbo(0),
width(width),
height(height),
color_tex(0)
{
	assert(width > 0 && height > 0);
	create_buffer();
}

Framebuffer::~Framebuffer()
{}

void Framebuffer::dispose()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteBuffers(1, &color_tex);
	fbo = 0;
	color_tex = 0;
}

void Framebuffer::create_buffer()
{
	assert(fbo == 0);
	
	// Gen framebuffer
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Gen color texture
	glGenTextures(1, &color_tex);
	glBindTexture(GL_TEXTURE_2D, color_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_tex, 0);

	if(true)
	{
		// Gen depth and stencil buffer
		glGenTextures(1, &depth_stencil_buffer);
		glBindTexture(GL_TEXTURE_2D, depth_stencil_buffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_stencil_buffer, 0);

	} 
	else 
	{
		// This is optimized version but can't be read-from, eg. if you want to show the textures
		// Gen depth and stencil buffer
		glGenRenderbuffers(1, &depth_stencil_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_buffer);
	}
	
	GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, draw_buffers);

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::prepare_render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//glViewport(0, 0, width, height); // @TODO borderless fullscreen needs different render-resolution
}

void Framebuffer::end_render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind_color_buffer()
{
	glBindTexture(GL_TEXTURE_2D, color_tex);
}



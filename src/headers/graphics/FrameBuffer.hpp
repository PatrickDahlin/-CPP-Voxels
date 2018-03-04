#ifndef _FRAMEBUFFER_HPP_
#define _FRAMEBUFFER_HPP_

#include "core/Common.hpp"
#include "core/Disposable.hpp"

class Framebuffer : public Disposable {
public:
	Framebuffer(int width, int height);
	virtual ~Framebuffer();

	void prepare_render();

	void end_render();

	void bind_color_buffer();

	void dispose() override;

	int get_width() const;

	int get_height() const;

private:

	void create_buffer();

	GLuint		fbo;
	int			width;
	int			height;

	GLuint		color_tex;
	GLuint		depth_stencil_buffer;

};

extern void render_framebuffer(Framebuffer* buf);

extern void cleanup_framebuffers();

#endif
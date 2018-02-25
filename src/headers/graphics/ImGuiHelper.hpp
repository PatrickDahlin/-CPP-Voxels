#ifndef _IMGUI_HELPER_HPP_
#define _IMGUI_HELPER_HPP_

#include <imgui/imgui.h>
#include <SDL2/SDL.h>
#include <string>

#include "graphics/ImGuiRenderer.hpp"
#include "graphics/GLTexture.hpp"
#include "graphics/ShaderProgram.hpp"
#include "graphics/OrthographicCamera.hpp"

#include "core/Files.hpp"

static ShaderProgram* imgui_shader = nullptr;
static OrthographicCamera* imgui_camera = nullptr;

static const char* ImGui_GetClipboardText(void*)
{
    return SDL_GetClipboardText();
}

static void ImGui_SetClipboardText(void*, const char* text)
{
    SDL_SetClipboardText(text);
}

void init_imgui(int fbo_width, int fbo_height)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(fbo_width,fbo_height);
	io.RenderDrawListsFn = render_imgui; 
	io.SetClipboardTextFn = ImGui_SetClipboardText;
	io.GetClipboardTextFn = ImGui_GetClipboardText;
	io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
    io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
    io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
    io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
    io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
    io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
    io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
    io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
    io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
    io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
	io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;

	// Set up font texture
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	GLTexture* myTex = new GLTexture(ColorFormat::RGBA, pixels, width, height);
	io.Fonts->TexID = (void*)myTex;

	std::string vert = read_file("data/shaders/ImGui-vert.glsl");
	std::string frag = read_file("data/shaders/ImGui-frag.glsl");
	std::string header = read_file("data/shaders/Shader_Header.glsl");
	imgui_shader = new ShaderProgram(vert.c_str(), frag.c_str(), header.c_str());

	imgui_camera = new OrthographicCamera(0, fbo_width, 0, fbo_height);

}

void dispose_imgui()
{
	ImGuiIO& io = ImGui::GetIO();

	// Delete font texture
	GLTexture* tex = nullptr;
	if(io.Fonts->TexID) tex = (GLTexture*)io.Fonts->TexID;
	if(tex) tex->dispose();
	if(tex) delete tex;
	tex = nullptr;

	if(imgui_shader)
	{	
		imgui_shader->dispose();
		delete imgui_shader; 
	}
}

void pre_render_imgui(float delta)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = delta;
	ImGui::NewFrame();
}

void post_render_imgui()
{
	imgui_shader->use();

	imgui_shader->upload_projection(imgui_camera->get_projection());
	imgui_shader->upload_view(imgui_camera->get_view());
	imgui_shader->upload_model(glm::mat4(1.0f));
	ImGui::Render();

}

bool request_input_imgui()
{
	ImGuiIO& io = ImGui::GetIO();
	return io.WantCaptureKeyboard || io.WantCaptureMouse;
}

void resize_imgui(int width, int height)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(width,height);

	delete imgui_camera;

	imgui_camera = new OrthographicCamera(0, width, 0, height);
}

#endif
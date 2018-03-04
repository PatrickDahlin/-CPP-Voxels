#include "core/SceneManager.hpp"
#include "core/Scene.hpp"
#include "graphics/ShaderManager.hpp"
#include "graphics/TextureManager.hpp"

SceneManager::SceneManager() :
last_width(0),
last_height(0), 
cur_scene(nullptr),
loaded_scenes()
{
	shader_manager = new ShaderManager();
	texture_manager = new TextureManager();
}

SceneManager::~SceneManager()
{}

void SceneManager::switch_to_scene(Scene* s)
{
	if(s == nullptr) return;

	if(cur_scene != nullptr)
		cur_scene->unload();

	// See if the scene already is initialized, if so don't re-initialize it
	bool is_init = false;
	for(Scene* s2 : loaded_scenes)
	{
		if(s2->getUUID() == s->getUUID())
		{
			is_init = true;
			break;
		}	
	}

	cur_scene = s;
	cur_scene->load(*shader_manager, *texture_manager);

	// Initialize and add to list only if it hasn't been initialized before
	if(!is_init)
	{
		cur_scene->init();
		cur_scene->resized_window(last_width, last_height);
		loaded_scenes.push_back(cur_scene);
	}
}

void SceneManager::update(const float delta)
{
	if(cur_scene)
		cur_scene->update(delta);
}

void SceneManager::render(RenderPass& pass)
{
	if(cur_scene)
		cur_scene->render(pass);
}

void SceneManager::resized_window(int width, int height)
{
	last_width = width;
	last_height = height;
	for(Scene* it : loaded_scenes)
	{	
		it->resized_window(width,height);
	}
}

void SceneManager::dispose()
{

	for(auto it : loaded_scenes)
	{
		it->unload();
		it->dispose();
		delete it;
	}
	
	cur_scene = nullptr;
	shader_manager->dispose();
	texture_manager->dispose();
}
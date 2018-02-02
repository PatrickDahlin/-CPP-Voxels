#include "core/SceneManager.hpp"
#include "core/Scene.hpp"

SceneManager::SceneManager() : 
cur_scene(nullptr),
loaded_scenes()
{
}

SceneManager::~SceneManager()
{}

void SceneManager::switch_to_scene(Scene* s)
{
	if(s == nullptr) return;

	bool is_loaded = false;
	for(Scene* s2 : loaded_scenes)

	{
		if(s2->getUUID() == s->getUUID())
		{
			is_loaded = true;
			break;
		}	
	}

	cur_scene = s;
	
	if(!is_loaded)
	{
		cur_scene->load();
		loaded_scenes.emplace_back(cur_scene);
		
	}
}

void SceneManager::init()
{
	cur_scene->init();
}

void SceneManager::update(const float delta)
{
	if(cur_scene)
		cur_scene->update(delta);
}

void SceneManager::render(RenderPass* pass)
{
	if(cur_scene)
		cur_scene->render(pass);
}

void SceneManager::dispose()
{
	if(cur_scene)
		cur_scene->dispose();
	
	for(auto it : loaded_scenes)
		it->dispose();

	for(auto it : loaded_scenes)
		if(it != nullptr)
			delete it;

	cur_scene = nullptr;
}
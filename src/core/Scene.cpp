#include "core/Scene.hpp"
#include "core/Macros.hpp"

int Scene::UUID_COUNTER = 0;

Scene::Scene(Input* input, SceneManager* scene_manager) :
UUID(UUID_COUNTER++),
input(input),
scene_manager(scene_manager)
{
	printf("Created new scene \t(uuid:%i)\n", UUID);
}

Scene::~Scene()
{
}

void Scene::dispose()
{
	// Dispose actors
	printf("Disposing scene \t(uuid:%i)\n",UUID);
}

int Scene::getUUID()
{
	return UUID;
}
#include "core/Scene.hpp"
#include "core/Macros.hpp"

int Scene::UUID_COUNTER = 0;

Scene::Scene() :
UUID(UUID_COUNTER++)
{
	cout("Created new scene (uuid:");
	cout(UUID);
	coutln(")");
}

Scene::~Scene()
{
}

void Scene::dispose()
{
	// Dispose actors
	printf("Disposing scene: %i\n",UUID);
}

int Scene::getUUID()
{
	return UUID;
}
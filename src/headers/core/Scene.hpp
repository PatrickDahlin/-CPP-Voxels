#ifndef _SCENE_HPP_
#define _SCENE_HPP_

class RenderPass;

class Scene
{
public:
	Scene();
	~Scene();

	virtual void load() = 0;
	// Effect: Loads resources from file
	// Postcond: All resources needed at initialization are loaded

	virtual void init() = 0;
	// Effect: Initializes this scene
	// Postcond: Scene is initialized

	virtual void update(const float delta) = 0;
	// Effect: Updates scene with actors
	// Postcond: All actors are updated

	virtual void render(RenderPass* pass) = 0;
	// Effect: Renders actors
	// Postcond: All actors are queued up for rendering

	virtual void dispose();
	// Effect: Disposes all actors and resources loaded
	// Postcond: Scene is "unloaded" and ready to be thrown away

private:

	// std::vector<Actor*>		actors;

};

#endif
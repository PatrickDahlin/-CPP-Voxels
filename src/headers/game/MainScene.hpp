#ifndef _MAINSCENE_HPP_
#define _MAINSCENE_HPP_

#include "core/Scene.hpp"

class RenderPass;

class MainScene : public Scene
{
public:
    MainScene();
    ~MainScene();

	void init() override;

    void load() override;

	void unload() override;

    void update(const float delta) override;

    void render(RenderPass* pass) override;

    void dispose() override;

private:

};


#endif
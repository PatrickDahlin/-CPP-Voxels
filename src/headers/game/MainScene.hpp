#ifndef _MAINSCENE_HPP_
#define _MAINSCENE_HPP_

#include "core/Scene.hpp"

class RenderPass;

class MainScene : public Scene
{
public:
    MainScene();
    ~MainScene();

    override void load();

    override void init();

    override void update(const float delta);

    override void render(RenderPass* pass);

    override void dispose();

private:

};


#endif
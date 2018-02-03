#include "game/MainScene.hpp"


MainScene::MainScene(Input* input, SceneManager* scene_manager) : Scene(input, scene_manager)
{}

MainScene::~MainScene()
{}

void MainScene::init()
{}

void MainScene::load()
{}

void MainScene::unload()
{}

void MainScene::update(const float delta)
{}

void MainScene::render(RenderPass* pass)
{}

void MainScene::dispose()
{
	Scene::dispose();
}




#include "core/Actor.hpp"
#include "graphics/Model.hpp"
#include "core/Transform.hpp"

Actor::Actor() : model(nullptr)
{}

Actor::~Actor()
{}

void Actor::set_model(Model* model)
{
	this->model = model;
}

Transform* Actor::get_transform()
{
	if(model)
		return &model->transform;
	else
		return nullptr;
}
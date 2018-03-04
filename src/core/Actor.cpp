#include "core/Actor.hpp"
#include "graphics/Model.hpp"
#include "core/Transform.hpp"
#include <algorithm>

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

void Actor::set_parent(Actor* parent)
{
	this->parent = parent;
}

void Actor::add_child(Actor* child)
{
	assert(child);
	if(std::find(children.begin(), children.end(), child) == children.end())
		children.push_back(child);
}

void Actor::remove_child(Actor* child)
{
	assert(child);
	auto it = std::find(children.begin(), children.end(), child);
	if(it != children.end())
		children.erase(it);
}

Actor* Actor::child_at(int index)
{
	assert(index >= 0 && index < (int)children.size());
	return children[index];
}

int Actor::child_count() const
{
	return children.size();
}
#ifndef _ACTOR_HPP_
#define _ACTOR_HPP_

#include "core/Common.hpp"
#include "core/Disposable.hpp"

#include <vector>

class Model;
class Transform;

class Actor : public Disposable {
public:
	Actor();
	virtual ~Actor();

	virtual void init() = 0;

	virtual void update(float delta) = 0;

	virtual void dispose() = 0;

	void set_parent(Actor* parent);
	void add_child(Actor* child);
	void remove_child(Actor* child);
	Actor* child_at(int index);
	int child_count() const;

	void set_model(Model* m);

	// Gets the transform of the model attached, nullptr if none is attached
	Transform* get_transform();

private:

	Actor*				parent;

	std::vector<Actor*>	children;
	Model*				model;

};

#endif
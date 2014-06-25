#include "GM/Framework/Entity.h"
#include "GM/Framework/Components/Transform.h"

#include <iostream>

using namespace GM::Framework;

std::atomic<unsigned long> Entity::next_id(0);

Entity::Entity(const std::string &name)
: id(next_id++)
, name(name)
, component_removed_slot(this, &Entity::on_component_removed)
{
	component_added().connect(clan::Callback<void(std::shared_ptr<IComponent<>>)>(this, &Entity::on_component_added));
	component_removed().connect(component_removed_slot);
}

Entity::~Entity() {
	// Since Entity inherits Totem::ComponentContainer we must avoid letting
	// it call methods on objects that are no longer valid (like ours)
	component_removed().disconnect(component_removed_slot);
	
	//Need to call this here so that Entity isn't destroyed when ComponentContainer invoke it's ComponentRemoved signals.
	pre_destruction();

	//std::cout << "Entity destroyed" << std::endl;
}

void Entity::on_component_added(std::shared_ptr<IComponent<>> component) {
	if (IComponent<>::is_type<Transform>(component)) {
		transform = std::static_pointer_cast<Transform>(component);
	}
}

void Entity::on_component_removed(std::shared_ptr<IComponent<>> component) {
	// This method must not get called in the destructor
	// Disconnect all signals to this method
	if (IComponent<>::is_type<Transform>(component)) {
		transform.reset();
	}
}

TransformPtr Entity::get_transform(std::function<void(const TransformPtr &)> func) const {
	if (func && has_transform()) func(transform);

	return transform;
}

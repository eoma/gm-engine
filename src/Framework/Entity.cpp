#include "GM/Framework/Entity.h"
#include "GM/Framework/Components/Transform.h"

#include <iostream>

using namespace GM::Framework;

Entity::Entity(const std::string &name)
: name(name)
, component_removed_slot(this, &Entity::on_component_removed)
{
	componentAdded().connect(clan::Callback<void(std::shared_ptr<Totem::IComponent<>>)>(this, &Entity::on_component_added));
	componentRemoved().connect(component_removed_slot);
}

Entity::~Entity() {
	// Since Entity inherits Totem::ComponentContainer we must avoid letting
	// it call methods on objects that are no longer valid (like ours)
	componentRemoved().disconnect(component_removed_slot);

	//std::cout << "Entity destroyed" << std::endl;
}

void Entity::on_component_added(std::shared_ptr<Totem::IComponent<>> component) {
	if (Totem::IComponent<>::isType<Transform>(component)) {
		transform = std::static_pointer_cast<Transform>(component);
	}
}

void Entity::on_component_removed(std::shared_ptr<Totem::IComponent<>> component) {
	// This method must not get called in the destructor
	// Disconnect all signals to this method
	if (Totem::IComponent<>::isType<Transform>(component)) {
		transform.reset();
	}
}

TransformPtr Entity::get_transform(std::function<void(const TransformPtr &)> func) const {
	if (func && has_transform()) func(transform);

	return transform;
}

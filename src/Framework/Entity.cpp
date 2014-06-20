#include "GM/Framework/Entity.h"
#include "GM/Framework/Components/Transform.h"

#include <iostream>

using namespace GM::Framework;

Entity::Entity(const std::string &name) : name(name) {
	componentAdded().connect(clan::Callback<void(std::shared_ptr<Totem::IComponent<>>)>(this, &Entity::on_component_added));
}

Entity::~Entity() {
	//std::cout << "Entity destroyed" << std::endl;
}

void Entity::on_component_added(std::shared_ptr<Totem::IComponent<>> component) {
	if (Totem::IComponent<>::isType<Transform>(component)) {
		transform = std::static_pointer_cast<Transform>(component);
	}
}

TransformPtr Entity::get_transform(std::function<void(const TransformPtr &)> func) const {
	if (func && has_transform()) func(transform);

	return transform;
}

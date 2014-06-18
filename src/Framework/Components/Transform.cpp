#include "GM/Framework/Components/Transform.h"

#include <glm/ext.hpp>

#include "GM/Framework/DefinitionsPropertyNames.h"
#include "GM/Framework/Entity.h"
#include "GM/Framework/Systems/SceneManager.h"

#include <algorithm>
#include <iostream>

using namespace GM::Framework;
using namespace Totem;

Transform::Transform(const EntityPtr &owner, SceneManagerPtr &scene_manager, const std::string &name)
: Component(name)
, owner(owner)
, scene_manager_weak(scene_manager)
{
	position_property = owner->add(PROPERTY_POSITION, glm::vec3());
	scale_property = owner->add(PROPERTY_SCALE, glm::vec3());
	orientation_property = owner->add(PROPERTY_ORIENTATION, glm::quat());

	scene_manager->add(shared_from_this());
}

Transform::~Transform() {
	auto current_parent = parent.lock();

	if (current_parent != nullptr) {
		current_parent->remove_child(shared_from_this());
	}

	// This will probably give you bad performance
	auto scene_manager = scene_manager_weak.lock();

	if (scene_manager != nullptr) {
		scene_manager->remove(shared_from_this());
	}
}

void Transform::add_child(const TransformPtr &child) {

	{
		// Remove previous parent and remove it from scene managers vector of parent less nodes
		auto current_parent = child->parent.lock();
		if (current_parent != nullptr) {
			std::cerr << "Child already has parent when getting new parent, please remove child before adding!";
			current_parent->remove_child(child);
		}
		
		auto scene_manager = scene_manager_weak.lock();
		if (scene_manager != nullptr) {
			scene_manager->remove(child);
		}
	}

	children.push_back(child);

	child->parent = shared_from_this();
}

void Transform::remove_child(const TransformPtr &child) {	
	auto iter = std::find(children.begin(), children.end(), child);

	if (iter != children.end()) {
		children.erase(iter);

		child->parent.reset();

		auto scene_manager = scene_manager_weak.lock();
		if (scene_manager != nullptr) {
			scene_manager->add(child);
		}
	}
}

TransformWeakPtr Transform::get_parent() const {
	return parent;
}

const std::vector<TransformPtr>& Transform::get_children() const {
	return children;
}

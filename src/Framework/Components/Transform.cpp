#include "GM/Framework/Components/Transform.h"

#include <glm/ext.hpp>

#include "GM/Framework/DefinitionsPropertyNames.h"
#include "GM/Framework/Entity.h"
#include "GM/Framework/Systems/SceneManager.h"

#include <algorithm>
#include <iostream>

using namespace GM::Framework;
using namespace Totem;

Transform::Transform(const EntityPtr &owner, const SceneManagerPtr &scene_manager, const std::string &name)
: Component(name)
, owner(owner)
, scene_manager(scene_manager)
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

	scene_manager->remove(shared_from_this());
}

void Transform::add_child(const TransformPtr &child) {
	scene_manager->add(child, shared_from_this(), add_callback, remove_callback);
}

void Transform::remove_child(const TransformPtr &child) {
	scene_manager->remove(child, shared_from_this(), remove_callback);
}

TransformWeakPtr Transform::get_parent() const {
	return parent;
}

const std::vector<TransformPtr>& Transform::get_children() const {
	return children;
}

bool Transform::has_parent() const {
	return parent.lock() != nullptr;
}
bool Transform::has_children() const {
	return !children.empty();
}

void Transform::add_callback(const TransformPtr &child, const TransformPtr &parent) {
	if (parent == nullptr || child == nullptr)
		return;

	parent->children.push_back(child);
	child->parent = parent;
}

void Transform::remove_callback(const TransformPtr &child, const TransformPtr &parent) {
	if (parent == nullptr || child == nullptr)
		return;

	auto iter = std::find(parent->children.begin(), parent->children.end(), child);

	if (iter != parent->children.end()) {
		parent->children.erase(iter);
		child->parent.reset();

		//The child has now become a parentless transform
		parent->scene_manager->add(child);
	}
}
#include "GM/Framework/Components/Transform.h"

#include <glm/ext.hpp>

#include "GM/Framework/DefinitionsPropertyNames.h"
#include "GM/Framework/Entity.h"
#include "GM/Framework/Systems/SceneManager.h"

#include <algorithm>
#include <iostream>

using namespace GM::Framework;

Transform::Transform(const EntityPtr &owner, const SceneManagerPtr &scene_manager, const std::string &name)
: Component<Transform>(owner, name)
, scene_manager(scene_manager)
, parent(nullptr)
{
	position_property = owner->add(PROPERTY_POSITION, glm::vec3());
	scale_property = owner->add(PROPERTY_SCALE, glm::vec3());
	orientation_property = owner->add(PROPERTY_ORIENTATION, glm::quat());

	object_matrix_property = owner->add(PROPERTY_OBJECT_MATRIX, glm::mat4());
	world_matrix_property = owner->add(PROPERTY_WORLD_MATRIX, glm::mat4());

	scene_manager->add(this);
}

Transform::~Transform() {
	//std::cout << "Transform destructor started" << std::endl;


	if (parent != nullptr) {
		parent->remove_child(this);
	}

	// Nodes are always added to the scene manager's parentLess transforms when removed from a parent
	scene_manager->remove(this);

	// Add all of the children to the scene manager.
	for (Transform * child : children) {
		child->parent = nullptr;
		scene_manager->add(child);
	}

	children.clear();

	//std::cout << "Transform destroyed" << std::endl;
}

void Transform::add_child(Transform * const child) {
	scene_manager->add(child, this, add_callback, remove_callback);
}

void Transform::remove_child(Transform * const child) {
	scene_manager->remove(child, this, remove_callback);
}

Transform* Transform::get_parent() const {
	return parent;
}

const std::vector<Transform *>& Transform::get_children() const {
	return children;
}

bool Transform::has_parent() const {
	return parent != nullptr;
}
bool Transform::has_children() const {
	return !children.empty();
}

void Transform::add_callback(Transform * const child, Transform * const parent) {
	if (parent == nullptr || child == nullptr) {
		return;
	}

	parent->children.push_back(child);
	child->parent = parent;

	parent->child_added_sig.invoke(parent, child);
}

void Transform::remove_callback(Transform * const child, Transform * const parent) {
	if (parent == nullptr || child == nullptr) {
		return;
	}

	auto iter = std::find(parent->children.begin(), parent->children.end(), child);

	if (iter != parent->children.end()) {
		parent->children.erase(iter);
		child->parent = nullptr;

		//The child has now become a parentless transform
		parent->scene_manager->add(child);

		parent->child_removed_sig.invoke(parent, child);
	}
}

bool Transform::is_dirty() const {
	return (position_property.is_dirty() || scale_property.is_dirty() || orientation_property.is_dirty());
}

void Transform::clear_dirty() {
	position_property.clear_dirty();
	scale_property.clear_dirty();
	orientation_property.clear_dirty();
}

void Transform::update_object_matrix() {
	object_matrix_property = make_object_matrix();
}

glm::mat4 Transform::make_object_matrix() const {
	glm::mat4 translation;
	translation[3] = glm::vec4(position_property.get(), 1);

	glm::mat4 rotation = glm::toMat4(orientation_property.get());

	glm::mat4 scale = glm::diagonal4x4(glm::vec4(scale_property.get(), 1));

	return translation * rotation * scale;
}

void Transform::update_world_matrix() {
	update_object_matrix();

	world_matrix_property = make_world_matrix();
}

glm::mat4 Transform::make_world_matrix() const {
	glm::mat4 thisWorld = get_object_matrix();

	if (parent != nullptr) {
		thisWorld = parent->get_world_matrix() * thisWorld;
	}

	return thisWorld;
}

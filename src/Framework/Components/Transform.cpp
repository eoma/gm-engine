#include "GM/Framework/Components/Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "GM/Framework/DefinitionsPropertyNames.h"
#include "GM/Framework/Entity.h"
#include "GM/Framework/Systems/SceneSystem.h"

#include <algorithm>
#include <iostream>

using namespace GM::Framework;

Transform::Transform(const EntityPtr &owner, const SceneSystemPtr &scene_system, const std::string &name)
: Component<Transform>(owner, name)
, scene_system(scene_system)
, parent(nullptr)
{
	position_property = owner->add(GM_PROPERTY_POSITION, glm::vec3());
	scale_property = owner->add(GM_PROPERTY_SCALE, glm::vec3(1));
	orientation_property = owner->add(GM_PROPERTY_ORIENTATION, glm::quat(
		glm::angleAxis(0.0f, glm::vec3(1, 0, 0)) *
		glm::angleAxis(0.0f, glm::vec3(0, 1, 0)) *
		glm::angleAxis(0.0f, glm::vec3(0, 0, 1))));

	object_matrix_property = owner->add(GM_PROPERTY_OBJECT_MATRIX, glm::mat4(1));
	world_matrix_property = owner->add(GM_PROPERTY_WORLD_MATRIX, glm::mat4(1));
	normal_matrix_property = owner->add(GM_PROPERTY_NORMAL_MATRIX, glm::mat3(1));
	object_matrix_no_scale_property = owner->add(GM_PROPERTY_OBJECT_MATRIX_NO_SCALE, glm::mat4(1));
	world_matrix_no_scale_property = owner->add(GM_PROPERTY_WORLD_MATRIX_NO_SCALE, glm::mat4(1));
	parent_world_matrix_no_scale_property = owner->add(GM_PROPERTY_PARENT_WORLD_MATRIX_NO_SCALE, glm::mat4(1));

	scene_system->add(this);
}

Transform::~Transform() {
	//std::cout << "Transform destructor started" << std::endl;


	if (parent != nullptr) {
		parent->remove_child(this);
	}

	// Nodes are always added to the scene manager's parentLess transforms when removed from a parent
	scene_system->remove(this);

	// Add all of the children to the scene manager.
	for (Transform * child : children) {
		child->parent = nullptr;
		scene_system->add(child);
	}

	children.clear();

	//std::cout << "Transform destroyed" << std::endl;
}

void Transform::add_child(Transform * const child) {
	scene_system->add(child, this, add_callback, remove_callback);
}

void Transform::remove_child(Transform * const child) {
	scene_system->remove(child, this, remove_callback);
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

	if (parent == child) {
		throw clan::Exception("Can not parent transform can not be child of itself!");
	}
	
	if (child_addition_forms_cycle(child, parent)) {
		throw clan::Exception(clan::string_format("Addition of child transform %1 to parent transform %2 forms a cycle!",
			child->get_owner()->get_name(), parent->get_owner()->get_name()));
	}

	parent->children.push_back(child);
	child->parent = parent;

	parent->child_added_sig(parent, child);
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
		parent->scene_system->add(child);

		parent->child_removed_sig(parent, child);
	}
}

bool Transform::child_addition_forms_cycle(Transform * const child, Transform * const parent) {
	// Assume child is a-cyclic, if it has children
	bool cyclic = false;
	Transform * candidate = parent;

	while (!cyclic && candidate != nullptr) {
		cyclic = (child == candidate);
		candidate = candidate->get_parent();
	}

	return cyclic;
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
	object_matrix_no_scale_property = make_object_matrix_no_scale();
}

glm::mat4 Transform::make_object_matrix() const {
	glm::mat4 translation = glm::translate(position_property.get());
	glm::mat4 rotation = glm::toMat4(orientation_property.get());
	glm::mat4 scale = glm::diagonal4x4(glm::vec4(scale_property.get(), 1));
	return translation * rotation * scale;
}

glm::mat4 Transform::make_object_matrix_no_scale() const {
	glm::mat4 translation = glm::translate(position_property.get());
	glm::mat4 rotation = glm::toMat4(orientation_property.get());
	return translation * rotation;
}

void Transform::update_world_matrix() {
	update_object_matrix();

	world_matrix_property = make_world_matrix();
	world_matrix_no_scale_property = make_world_matrix_no_scale();

	if (parent != nullptr) {
		parent_world_matrix_no_scale_property = parent->get_world_matrix_no_scale();
	}
}

glm::mat4 Transform::make_world_matrix() const {
	glm::mat4 this_world = get_object_matrix();

	if (parent != nullptr) {
		this_world = parent->get_world_matrix_no_scale() * this_world;
	}

	return this_world;
}

glm::mat4 Transform::make_world_matrix_no_scale() const {
	glm::mat4 this_world_no_scale = get_object_matrix_no_scale();

	if (parent != nullptr) {
		this_world_no_scale = parent->get_world_matrix_no_scale() * this_world_no_scale;
	}

	return this_world_no_scale;
}


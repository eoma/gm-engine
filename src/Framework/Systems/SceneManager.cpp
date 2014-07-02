#include "GM/Framework/Systems/SceneManager.h"
#include "GM/Framework/Components/Transform.h"

#include <algorithm>
#include <iostream>

using namespace GM::Framework;

SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {
	//std::cout << "SceneManager destroyed" << std::endl;
}

void SceneManager::add(Transform * const transform) {
	if (transform == nullptr) {
		return;
	}

	auto iter = std::find(parentlessTransforms.begin(), parentlessTransforms.end(), transform);

	if (iter == parentlessTransforms.end()) {
		parentlessTransforms.push_back(transform);
	}
}

void SceneManager::add(Transform * const transform, Transform * const parent) {
	if (transform == nullptr) {
		return;
	} else if (parent == nullptr) {
		add(transform);
	} else {
		add(transform, parent, Transform::add_callback, Transform::remove_callback);
	}
}

void SceneManager::add(Transform * const transform, Transform * const parent,
	std::function<void(Transform * const child, Transform * const parent)> add_callback,
	std::function<void(Transform * const child, Transform * const parent)> remove_callback) {
	if (transform == nullptr) {
		return;
	} else if (parent == nullptr) {
		add(transform);
	} else {
		//First remove from parentless list or remove from previous parent before we set the new one.
		remove(transform, transform->get_parent(), remove_callback);

		//Then define new parent for transform
		//parent->add_child(transform);
		if (add_callback) {
			add_callback(transform, parent);
		}
	}
}

void SceneManager::remove(Transform * const transform) {
	if (transform == nullptr) {
		return;
	}

	auto iter = std::find(parentlessTransforms.begin(), parentlessTransforms.end(), transform);

	if (iter != parentlessTransforms.end()) {
		parentlessTransforms.erase(iter);
	}
}

void SceneManager::remove(Transform * const transform, Transform * const parent) {
	if (transform == nullptr) {
		return;
	} else if (parent == nullptr) {
		remove(transform);
	} else {
		remove(transform, parent, Transform::remove_callback);
	}
}

void SceneManager::remove(Transform * const transform, Transform * const parent,
	std::function<void(Transform * const child, Transform * const parent)> remove_callback) {
	if (transform == nullptr) {
		return;
	} else if (parent == nullptr) {
		remove(transform);
	} else {
		if (remove_callback) {
			remove_callback(transform, parent);
		}
	}
}

const std::vector<Transform*> &SceneManager::get_transforms() const {
	return parentlessTransforms;
}

void SceneManager::prepare() {
	for (Transform *transform : parentlessTransforms) {
		prepare(transform);
	}

}

void SceneManager::prepare(Transform *transform, bool must_update_world) {
	if (transform->is_dirty()) {
		transform->update_object_matrix();
		transform->clear_dirty();

		must_update_world = true;
	}

	if (must_update_world) {
		transform->update_world_matrix();
	}

	for (Transform *child : transform->get_children()) {
		prepare(child, must_update_world);
	}
}

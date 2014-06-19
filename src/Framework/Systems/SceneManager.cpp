#include "GM/Framework/Systems/SceneManager.h"
#include "GM/Framework/Components/Transform.h"

#include <algorithm>

using namespace GM::Framework;

SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {

}

void SceneManager::add(const TransformPtr &transform) {
	if (transform == nullptr) {
		return;
	}

	auto iter = std::find(parentlessTransforms.begin(), parentlessTransforms.end(), transform);

	if (iter == parentlessTransforms.end()) {
		parentlessTransforms.push_back(transform);
	}
}

void SceneManager::add(const TransformPtr &transform, const TransformPtr &parent) {
	if (transform == nullptr) {
		return;
	} else if (parent == nullptr) {
		add(transform);
	} else {
		add(transform, parent, Transform::add_callback, Transform::remove_callback);
	}
}

void SceneManager::add(const TransformPtr &transform, const TransformPtr &parent, 
	std::function<void(const TransformPtr &child, const TransformPtr &parent)> add_callback, 
	std::function<void(const TransformPtr &child, const TransformPtr &parent)> remove_callback) {
	if (transform == nullptr) {
		return;
	} else if (parent == nullptr) {
		add(transform);
	} else {
		//First remove from parentless list or remove from previous parent before we set the new one.
		remove(transform, transform->get_parent().lock(), remove_callback);

		//Then define new parent for transform
		//parent->add_child(transform);
		if (add_callback) {
			add_callback(transform, parent);
		}
	}
}

void SceneManager::remove(const TransformPtr &transform) {
	if (transform == nullptr) {
		return;
	}

	auto iter = std::find(parentlessTransforms.begin(), parentlessTransforms.end(), transform);

	if (iter != parentlessTransforms.end()) {
		parentlessTransforms.erase(iter);
	}
}

void SceneManager::remove(const TransformPtr &transform, const TransformPtr &parent) {
	if (transform == nullptr) {
		return;
	} else if (parent == nullptr) {
		remove(transform);
	} else {
		remove(transform, parent, Transform::remove_callback);
	}
}

void SceneManager::remove(const TransformPtr &transform, const TransformPtr &parent, 
	std::function<void(const TransformPtr &child, const TransformPtr &parent)> remove_callback) {
	if (transform == nullptr) {
		return;
	} else if (parent == nullptr) {
		remove(transform);
	} else {
		if (remove_callback) {
			remove_callback(transform, parent);
		}
	}
}

const std::vector<TransformPtr> &SceneManager::get_transforms() const {
	return parentlessTransforms;
}

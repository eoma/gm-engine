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

void SceneManager::add(Transform *transform) {
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
		add(transform.get());
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
		add(transform.get());
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

void SceneManager::remove(Transform *transform) {
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
		remove(transform.get());
	} else {
		remove(transform, parent, Transform::remove_callback);
	}
}

void SceneManager::remove(const TransformPtr &transform, const TransformPtr &parent, 
	std::function<void(const TransformPtr &child, const TransformPtr &parent)> remove_callback) {
	if (transform == nullptr) {
		return;
	} else if (parent == nullptr) {
		remove(transform.get());
	} else {
		if (remove_callback) {
			remove_callback(transform, parent);
		}
	}
}

const std::vector<Transform*> &SceneManager::get_transforms() const {
	return parentlessTransforms;
}

#include "GM/Framework/Systems/SceneManager.h"

#include <algorithm>

using namespace GM::Framework;

SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {

}

void SceneManager::add(const TransformPtr &transform) {
	auto iter = std::find(parentlessTransforms.begin(), parentlessTransforms.end(), transform);

	if (iter == parentlessTransforms.end()) {
		parentlessTransforms.push_back(transform);
	}
}

void SceneManager::remove(const TransformPtr &transform) {
	auto iter = std::find(parentlessTransforms.begin(), parentlessTransforms.end(), transform);

	if (iter != parentlessTransforms.end()) {
		parentlessTransforms.erase(iter);
	}
}

const std::vector<TransformPtr> &SceneManager::get_transforms() const {
	return parentlessTransforms;
}

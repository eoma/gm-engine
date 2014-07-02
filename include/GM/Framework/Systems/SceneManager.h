#pragma once

#include <memory>
#include <vector>
#include <functional>

namespace GM {
namespace Framework {

class Transform; typedef std::shared_ptr<Transform> TransformPtr;

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	// Prepare the transform
	void prepare();

	void add(Transform * const transform);
	void add(Transform * const child, Transform * const parent);
	void add(Transform * const transform, Transform * const parent,
		std::function<void(Transform * const child, Transform * const parent)> add_callback,
		std::function<void(Transform * const child, Transform * const parent)> remove_callback);

	void remove(Transform * const transform);
	void remove(Transform * const parent, Transform * const child);
	void remove(Transform * const transform, Transform * const parent,
		std::function<void(Transform * const child, Transform * const parent)> remove_callback);

	const std::vector<Transform*> &get_transforms() const;

private:
	// Walk through the tree and update object and world matrices, if necessary
	void prepare(Transform *transform, bool must_update_world = false);

private:
	// SceneManager will not attempt to destruct these when it is itself destructed.
	std::vector<Transform*> parentlessTransforms;

};

typedef std::shared_ptr<SceneManager> SceneManagerPtr;

} // namespace Framework
} // namespace GM

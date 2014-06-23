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
	// SceneManager will not attempt to destruct these when it is itself destructed.
	std::vector<Transform*> parentlessTransforms;

};

} // namespace Framework
} // namespace GM

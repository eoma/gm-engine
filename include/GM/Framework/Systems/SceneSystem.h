#pragma once

#include <memory>
#include <vector>
#include <functional>

namespace GM {
namespace Framework {

class Transform; typedef std::shared_ptr<Transform> TransformPtr;

class SceneSystem {
public:
	SceneSystem();
	~SceneSystem();

	/**
	 * Traverse the scene tree and
	 *
	 * @param force_update force all transforms to update it's values whether dirty or not
	 */
	void prepare(bool force_update = false);

	void add(Transform * const transform);
	void add(Transform * const child, Transform * const parent);
	void add(Transform * const transform, Transform * const parent,
		std::function<void(Transform * const child, Transform * const parent)> add_callback,
		std::function<void(Transform * const child, Transform * const parent)> remove_callback);

	void remove(Transform * const transform);
	void remove(Transform * const parent, Transform * const child);
	void remove(Transform * const transform, Transform * const parent,
		std::function<void(Transform * const child, Transform * const parent)> remove_callback);

	/**
	 * Get a list of all parentless transforms. These forms the root of the tree.
	 *
	 * @return list of parentless transforms.
	 */
	const std::vector<Transform*> &get_transforms() const;

private:
	/**
	 * Walk through the tree and update object and world matrices, if necessary.
	 */
	void prepare(Transform *transform, bool must_update_world = false, bool force_update = false);

private:
	// SceneManager will not attempt to destruct these when it is itself destructed.
	std::vector<Transform*> parentless_transforms;

};

typedef std::shared_ptr<SceneSystem> SceneSystemPtr;

} // namespace Framework
} // namespace GM

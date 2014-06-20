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

	void add(Transform *transform);
	void add(const TransformPtr &child, const TransformPtr &parent);
	void add(const TransformPtr &transform, const TransformPtr &parent, 
		std::function<void(const TransformPtr &child, const TransformPtr &parent)> add_callback,
		std::function<void(const TransformPtr &child, const TransformPtr &parent)> remove_callback);

	void remove(Transform *transform);
	void remove(const TransformPtr &parent, const TransformPtr &child);
	void remove(const TransformPtr &transform, const TransformPtr &parent, 
		std::function<void(const TransformPtr &child, const TransformPtr &parent)> remove_callback);

	const std::vector<Transform*> &get_transforms() const;

private:
	// SceneManager will not attempt to destruct these when it is itsels destructed.
	std::vector<Transform*> parentlessTransforms;

};

} // namespace Framework
} // namespace GM

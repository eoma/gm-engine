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

	void add(const TransformPtr &transform);
	void add(const TransformPtr &child, const TransformPtr &parent);
	void add(const TransformPtr &transform, const TransformPtr &parent, 
		std::function<void(TransformPtr child, TransformPtr parent)> add_callback, 
		std::function<void(TransformPtr child, TransformPtr parent)> remove_callback);

	void remove(const TransformPtr &transform);
	void remove(const TransformPtr &parent, const TransformPtr &child);
	void remove(const TransformPtr &transform, const TransformPtr &parent, 
		std::function<void(TransformPtr child, TransformPtr parent)> remove_callback);

	const std::vector<TransformPtr> &get_transforms() const;

private:
	std::vector<TransformPtr> parentlessTransforms;

};

} // namespace Framework
} // namespace GM

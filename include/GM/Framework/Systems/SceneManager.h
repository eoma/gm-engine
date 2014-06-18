#pragma once

#include <memory>
#include <vector>

namespace GM {
namespace Framework {

class Transform; typedef std::shared_ptr<Transform> TransformPtr;

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	void add(const TransformPtr &transform);
	//void add_child(const TransformPtr &parent, const TransformPtr &child);

	void remove(const TransformPtr &transform);
	//void remove_child(const TransformPtr &parent, const TransformPtr &child);

	const std::vector<TransformPtr> &get_transforms() const;

private:
	std::vector<TransformPtr> parentlessTransforms;

};

} // namespace Framework
} // namespace GM

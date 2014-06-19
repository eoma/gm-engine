#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

// Totem
#include <Totem/Component.h>

// STL
#include <memory>
#include <string>
#include <vector>

namespace GM {
namespace Framework {

class SceneManager; typedef std::shared_ptr<SceneManager> SceneManagerPtr; typedef std::weak_ptr<SceneManager> SceneManagerWeakPtr;
class Entity; typedef std::shared_ptr<Entity> EntityPtr;
class Transform; typedef std::shared_ptr<Transform> TransformPtr; typedef std::weak_ptr<Transform> TransformWeakPtr;

class Transform : public Totem::Component<Transform>, public std::enable_shared_from_this<Transform>
{
public:
	Transform(const EntityPtr &owner, const SceneManagerPtr &scene_manager, const std::string &name = std::string());
	~Transform();

	void add_child(const TransformPtr &child);
	void remove_child(const TransformPtr &child);

	TransformWeakPtr get_parent() const;
	const std::vector<TransformPtr>& get_children() const;

	bool has_parent() const;
	bool has_children() const;

public:
	static void add_callback(const TransformPtr &child, const TransformPtr &parent);
	static void remove_callback(const TransformPtr &child, const TransformPtr &parent);

private:
	EntityPtr owner;
	SceneManagerPtr scene_manager;

	TransformWeakPtr parent;
	std::vector<TransformPtr> children;

	// TODO: Proper defs. of variables
	Totem::Property<glm::vec3> position_property;
	Totem::Property<glm::vec3> scale_property;
	Totem::Property<glm::quat> orientation_property;
};

} // namespace Framework
} // namespace GM
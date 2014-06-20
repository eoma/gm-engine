#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

// ClanLib
#include <ClanLib/core.h>

// Totem
#include <Totem/Component.h>

// STL
#include <memory>
#include <string>
#include <vector>

namespace GM {
namespace Framework {

class SceneManager; typedef std::shared_ptr<SceneManager> SceneManagerPtr; typedef std::weak_ptr<SceneManager> SceneManagerWeakPtr;
class Entity; typedef std::shared_ptr<Entity> EntityPtr; typedef std::weak_ptr<Entity> EntityWeakPtr;
class Transform; typedef std::shared_ptr<Transform> TransformPtr; typedef std::weak_ptr<Transform> TransformWeakPtr;

class Transform : public Totem::Component<Transform>, public std::enable_shared_from_this<Transform>
{
public:
	Transform(const EntityPtr &owner, const SceneManagerPtr &scene_manager, const std::string &name = std::string());
	virtual ~Transform();

	void add_child(const TransformPtr &child);
	void remove_child(const TransformPtr &child);

	// These signals are invoked when a child is added or removed from this transform.
	// Ther order of parameters: parent, then child
	clan::Signal<const TransformPtr&, const TransformPtr&> get_child_added_signal() const { return child_added_sig; };
	clan::Signal<const TransformPtr&, const TransformPtr&> get_child_removed_signal() const { return child_removed_sig; };

	TransformWeakPtr get_parent() const;
	const std::vector<TransformPtr>& get_children() const;

	bool has_parent() const;
	bool has_children() const;

	const glm::vec3 &get_position() const { return position_property; }
	const glm::vec3 &get_scale() const { return scale_property; }
	const glm::quat &get_orientation() const { return orientation_property; }

	void set_position(const glm::vec3 &position) { position_property = position; }
	void set_scale(const glm::vec3 &scale) { scale_property = scale; }
	void set_orientation(const glm::quat &orientation) { orientation_property = orientation; }

	void translate(const glm::vec3 &position) { position_property += position; }
	void scale(const glm::vec3 &scale) { scale_property += scale; }
	void rotate(const glm::quat &orientation) { orientation_property *= orientation; }

public:

	// Called when a child is to be added to a parent
	static void add_callback(const TransformPtr &child, const TransformPtr &parent);

	// Called when a child is to be removed from a parent
	static void remove_callback(const TransformPtr &child, const TransformPtr &parent);

private:
	EntityWeakPtr owner;
	SceneManagerPtr scene_manager;

	TransformWeakPtr parent;
	std::vector<TransformPtr> children;

	// Signal is invoked when a new child is added to or removed from the transform
	// First argument is parent, second is child.
	clan::Signal<const TransformPtr&, const TransformPtr&> child_added_sig;
	clan::Signal<const TransformPtr&, const TransformPtr&> child_removed_sig;

	// TODO: Proper defs. of variables
	Totem::Property<glm::vec3> position_property;
	Totem::Property<glm::vec3> scale_property;
	Totem::Property<glm::quat> orientation_property;
};

} // namespace Framework
} // namespace GM

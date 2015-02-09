#pragma once

#include "../DefinitionsComponentNames.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// ClanLib
#include <ClanLib/core.h>

// Totem
#include "../Totem/Component.h"

// STL
#include <memory>
#include <string>
#include <vector>

namespace GM {
namespace Framework {

class SceneSystem; typedef std::shared_ptr<SceneSystem> SceneSystemPtr; typedef std::weak_ptr<SceneSystem> SceneSystemWeakPtr;
class Transform; typedef std::shared_ptr<Transform> TransformPtr; typedef std::weak_ptr<Transform> TransformWeakPtr;

class Transform : public Component<Transform>, public std::enable_shared_from_this<Transform>
{
public:
	Transform(const EntityPtr &owner, const SceneSystemPtr &scene_system, const std::string &name = std::string());
	virtual ~Transform();

	std::string get_type() const override { return get_static_type(); }

	/**
	 * Scene tree relevant methods
	 */

	/**
	 * Add a Transform as a direct child
	 */
	void add_child(const TransformPtr &child) { add_child( child.get() ); };
	void add_child(Transform * const child);

	/**
	 * Remove a transform as a direct child
	 */
	void remove_child(const TransformPtr &child) { remove_child( child.get() ); };
	void remove_child(Transform * const child);

	/**
	 * These signals are invoked when a child is added or removed from this transform.
	 * Their order of parameters: parent, then child
	 */
	clan::Signal<void(const Transform* const, const Transform* const)> &get_child_added_signal() { return child_added_sig; };
	clan::Signal<void(const Transform* const, const Transform* const)> &get_child_removed_signal() { return child_removed_sig; };

	/**
	 * Get current parent, if no parent exists it will return nullptr
	 *
	 * @return currrent parent or nullptr if not set.
	 */
	Transform* get_parent() const;
	const std::vector<Transform*>& get_children() const;

	/**
	 * Check if this transform has a parent
	 */
	bool has_parent() const;

	/**
	 * Check if this transform has children
	 */
	bool has_children() const;

	//
	//
	//


	/**
	 * Check whether position, orientation or scale is dirty.
	 */
	bool is_dirty() const;

	/**
	 * Clear dirty flag on the position, orientation and scale properties
	 */
	void clear_dirty();

	// Position and orientation is always relative to this Transforms's (possible) parent
	// Scale will only work on _this_ object. It will not be passed on to children.
	const glm::vec3 &get_position() const { return position_property; }
	const glm::vec3 &get_scale() const { return scale_property; }
	const glm::quat &get_orientation() const { return orientation_property; }

	void set_position(const glm::vec3 &position) { position_property = position; }
	void set_scale(const glm::vec3 &scale) { scale_property = scale; }
	void set_orientation(const glm::quat &orientation) { orientation_property = orientation; }

	void translate(const glm::vec3 &position) { position_property += position; }
	void scale(const glm::vec3 &scale) { scale_property += scale; }
	void rotate(const glm::quat &orientation) { orientation_property *= orientation; }

	/**
	 * Being able to get position in world space is useful for systems 
	 * (like moving a "disconnected" camera to a light's position)
	 * Currently computed everytime it is needed.
	 *
	 * @return position in world space
	 */
	glm::vec3 get_position_in_worldspace() const;

	/**
	 * Returns the orientation property converted to a 3x3 matrix. Read only
	 *
	 * @return the orientation matrix
	 */
	const glm::mat3 &get_orientation_matrix() const { return orientation_matrix_property; }

	/**
	 * Get current world matrix. Will only be updated after update_world_matrix() has been called.
	 */
	const glm::mat4 &get_world_matrix() const { return world_matrix_property; };

	/**
	 * Get current world matrix without the scale property applied.
	 * Used mostly by SceneSystem.
	 *
	 * @return world matrix without scale
	 */
	const glm::mat4 &get_world_matrix_no_scale() const { return world_matrix_no_scale_property; };

	/**
	 * Updates world matrix and world matrix without scale.
	 *
	 * world matrix = parent world matrix without scale * object matrix.
	 */
	void update_world_matrix();

	/**
	 * Get current object matrix.
	 *
	 * @return object matrix
	 */
	const glm::mat4 &get_object_matrix() const { return object_matrix_property; };

	/**
	 * Get object matrix without scale.
	 *
	 * @return object matrix without scale
	 */
	const glm::mat4 &get_object_matrix_no_scale() const { return object_matrix_no_scale_property; };

	/**
	 * Updates object matrix and object matrix without scale.
	 * Sets object_matrix_property = make_object_matrix() if dependent 
	 * properties (position, orientation, scale) has changed.
	 */
	void update_object_matrix();

public:
	static std::string get_static_type() { return GM_COMPONENT_TRANSFORM; }

	// Called when a child is to be added to a parent
	static void add_callback(Transform * const child, Transform * const parent);

	// Called when a child is to be removed from a parent
	static void remove_callback(Transform * const child, Transform * const parent);

	static bool child_addition_forms_cycle(Transform * const child, Transform * const parent);

protected:
	glm::mat4 make_object_matrix() const;
	glm::mat4 make_object_matrix_no_scale() const;
	glm::mat4 make_world_matrix() const;
	glm::mat4 make_world_matrix_no_scale() const;

private:
	SceneSystemPtr scene_system;

	Transform * parent;
	std::vector<Transform *> children;

	// Signal is invoked when a new child is added to or removed from the transform
	// First argument is parent, second is child.
	clan::Signal<void(const Transform* const, const Transform* const)> child_added_sig;
	clan::Signal<void(const Transform* const, const Transform* const)> child_removed_sig;

	// TODO: Proper defs. of variables
	Property<glm::vec3> position_property;
	Property<glm::vec3> scale_property;
	Property<glm::quat> orientation_property;

	Property<glm::mat4> object_matrix_property;
	Property<glm::mat4> object_matrix_no_scale_property;
	Property<glm::mat4> world_matrix_property; // = parent->world_matrix_no_scale * this->object_matrix
	Property<glm::mat4> world_matrix_no_scale_property;
	Property<glm::mat4> parent_world_matrix_no_scale_property;

	Property<glm::mat3> normal_matrix_property;

	// Read only, as in it will only change when orientation_property changes
	Property<glm::mat3> orientation_matrix_property;

	clan::Slot orientation_change_slot;
};

} // namespace Framework
} // namespace GM

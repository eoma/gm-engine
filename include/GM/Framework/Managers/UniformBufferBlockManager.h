#pragma once

#include "../../Core/Utilities/BufferAllocation.h"

#include <map>
#include <vector>
#include <memory>

namespace GM {
namespace Core {
	class Shader; typedef std::shared_ptr<Shader> ShaderPtr;
} // namespace Core

namespace Framework {

/**
 * The uniform buffer block manager's primary concern is controlling which
 * named uniform buffer block binds to a specific binding block, and which buffer
 * binds to a specific binding point.
 */
class UniformBufferBlockManager {
public:
	/**
	 * Construct an instance of the uniform buffer block manager.
	 * @param initialize_constants whtehter to query the OpenGL layer for uniform buffer block constants.
	 */
	UniformBufferBlockManager(bool initialize_constants = true);

	~UniformBufferBlockManager();

	/**
	 * Read instance data from the current OpenGL context, should only be called if
	 * this object was instantiated before a context was setup
	 */
	void initialize();

	/**
	 * Are we initialized?
	 */
	bool is_initialized() const;

	/**
	 * Register a named buffer block, but don't associate a buffer yet.
	 * If binding_point is non_negative, it will use that as the actual binding point.
	 * If binding_point in negative, the binding point will be auto generated.
	 *
	 * @return true if name did not exist, false otherwise
	 */
	bool register_named_buffer_block(const std::string &name, int requested_binding_point = -1);

	/**
	 * Check if this instance has a named buffer block.
	 */
	bool has_named_buffer_block(const std::string &name) const;

	/**
	 * Returns whether a buffer allocation has been set for a given binding point.
	 */
	bool has_buffer_for_binding(int binding_pointr) const;

	/**
	 * Check if this instance has a certain binding point registered.
	 */
	bool has_binding_point(int binding_point) const;

	/**
	 * Set a named buffer block to a given allocation. Will bind the buffer to the registered
	 * binding point. If name is not registered, it will be created with a vacant binding point.
	 * Will print a warning if allocation was already set.
	 */
	void set_buffer_block(const std::string &name, const Core::BufferAllocation &allocation);

	/**
	 * Set a binding point to a given allocation. Will print a warning if allocation
	 * was already set.
	 */
	void set_buffer_block(int binding_point, const Core::BufferAllocation &allocation);

	/**
	 * Get binding point of named block. Throws an exception if it doesn't exist.
	 */
	int get_binding_point(const std::string &name) const;

	/**
	 * Get buffer allocation for a given buffer block name. Will throw if it doesn't exist.
	 */
	Core::BufferAllocation get_buffer_allocation(const std::string &name) const;

	/**
	 * Get buffer allocation for a given binding point. Will throw if it doesn't exist.
	 */
	Core::BufferAllocation get_buffer_allocation(int binding_point) const;

	/**
	 * Scans the shader's uniform buffer blocks and binds them, even if no buffer allocation has been set.
	 */
	void bind_shader(const Core::ShaderPtr &shader);

private:	
	int get_vacant_binding_point();

	void mark_binding_point_as_used(int binding_point);

private:
	bool initialized;

	// Maximum number of binding points is based on the relevant GL implementation
	// Sum of max binding points for vertex, geometry and fragment stage.
	int max_binding_points;

	std::vector<int> used_binding_points;

	std::map<int, Core::BufferAllocation> bound_allocation;

	std::map<std::string, int> named_buffer_blocks;
};

} // namespace Framework
} // namespace GM

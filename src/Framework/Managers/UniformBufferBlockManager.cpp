#include "GM/Framework/Managers/UniformBufferBlockManager.h"

#include "GM/Core/GL/Shader.h"

#include "ClanLib/core.h"

#include "GL/gl3w.h"

#include <algorithm>

namespace GM {
namespace Framework {

UniformBufferBlockManager::UniformBufferBlockManager(bool initialize_constants)
: initialized(false)
, max_binding_points(84)
{
	if (initialize_constants)
	{
		initialize();
	}
}

UniformBufferBlockManager::~UniformBufferBlockManager()
{
}

void UniformBufferBlockManager::initialize()
{
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &max_binding_points);

	initialized = true;
}

bool UniformBufferBlockManager::is_initialized() const
{
	return initialized;
}

bool UniformBufferBlockManager::register_named_buffer_block(const std::string &name, int requested_binding_point)
{
	auto iter = named_buffer_blocks.find(name);
	bool name_is_new = (iter == named_buffer_blocks.end());

	if (name_is_new)
	{
		int binding_point = requested_binding_point;

		if (binding_point < 0)
		{
			binding_point = get_vacant_binding_point();
		}

		if (binding_point >= max_binding_points)
		{
			throw clan::Exception(clan::string_format(
					"Binding point %1 is equal or larger than max number of "
					"binding points %2", binding_point, max_binding_points));
		}

		if (!has_binding_point(binding_point))
		{
			mark_binding_point_as_used(binding_point);
		}

		named_buffer_blocks.emplace(name, binding_point);
	}
	else if (requested_binding_point >= 0 && named_buffer_blocks[name] != requested_binding_point)
	{
		// Name exists, but requested binding point is different than what is already set.
		throw clan::Exception(clan::string_format(
				"Named uniform buffer block %1 has different binding point (%2)"
				"than what is already set (%3)", name, requested_binding_point,
				named_buffer_blocks[name]));
	}

	return name_is_new;
}

bool UniformBufferBlockManager::has_named_buffer_block(const std::string &name) const
{
	return named_buffer_blocks.find(name) != named_buffer_blocks.end();
}

bool UniformBufferBlockManager::has_buffer_for_binding(int binding_point) const
{
	return bound_allocation.find(binding_point) != bound_allocation.end();
}

bool UniformBufferBlockManager::has_binding_point(int binding_point) const
{
	return std::binary_search(used_binding_points.begin(), used_binding_points.end(), binding_point);
}

void UniformBufferBlockManager::set_buffer_block(const std::string &name, const Core::BufferAllocation &allocation)
{
	if (!has_named_buffer_block(name))
	{
		register_named_buffer_block(name);
	}
	set_buffer_block(named_buffer_blocks.at(name), allocation);
}

void UniformBufferBlockManager::set_buffer_block(int binding_point, const Core::BufferAllocation &allocation)
{
	auto iter = bound_allocation.find(binding_point);

	if (iter == bound_allocation.end())
	{
		iter = bound_allocation.emplace(binding_point, allocation).first;
	}
	else
	{
		iter->second = allocation;

		std::clog << clan::string_format("Replacing buffer at binding point %1", binding_point) << std::endl;
	}

	// TODO: ensure offset is a multiple of GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT?
	// Will get message from GL either way
	glBindBufferRange(GL_UNIFORM_BUFFER, binding_point, allocation.buffer->get_handle(),
	                  allocation.offset, allocation.allocated_size);
}

Core::BufferAllocation UniformBufferBlockManager::get_buffer_allocation(const std::string &name) const
{
	return get_buffer_allocation(named_buffer_blocks.at(name));
}

Core::BufferAllocation UniformBufferBlockManager::get_buffer_allocation(int binding_point) const
{
	return bound_allocation.at(binding_point);
}

int UniformBufferBlockManager::get_binding_point(const std::string &name) const
{
	return named_buffer_blocks.at(name);
}

int UniformBufferBlockManager::get_vacant_binding_point()
{
	// Will find a binding point that is not used.
	// Will search through used_binding_points for a gap between numbers
	// and insert a fitting number (usually the consecutive number).

	// TODO: Find a better data structure and search method to locate gaps between intervals

	// Initial binding point value.
	int binding_point = 0;

	if (used_binding_points.size() == 1 && used_binding_points.front() == binding_point)
	{
		// There are only one element in the used bindings array and the first point
		// is equal to the initial value.
		binding_point = used_binding_points.front() + 1;
	}
	else if (used_binding_points.size() > 1 && binding_point >= used_binding_points.front())
	{
		// Search for the first element with a gap larger than 1, between itself and the next element.
		auto iter = std::adjacent_find(used_binding_points.begin(), used_binding_points.end(),
		                               [](int first, int second) { return (second - first) > 1; });

		if (iter == used_binding_points.end())
		{
			binding_point = used_binding_points.back() + 1;
		}
		else
		{
			binding_point = (*iter) + 1;
		}
	}

	mark_binding_point_as_used(binding_point);

	return binding_point;
}

void UniformBufferBlockManager::mark_binding_point_as_used(int binding_point)
{
	used_binding_points.push_back(binding_point);
	std::sort(used_binding_points.begin(), used_binding_points.end());
}

void UniformBufferBlockManager::bind_shader(const Core::ShaderPtr &shader)
{
	for (const Core::UniformBlockInfo &info : shader->get_uniform_block_infos())
	{
		register_named_buffer_block(info.name);
		int binding_point = get_binding_point(info.name);

		// Perform the binding
		glUniformBlockBinding(shader->get_handle(), info.index, binding_point);
	}
}	

} // namespace Framework
} // namespace GM

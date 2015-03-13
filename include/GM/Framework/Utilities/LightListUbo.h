#pragma once

#include "GM/Core/Utilities/BufferAllocation.h"

#include <glm/vec4.hpp>

namespace GM {
namespace Framework {

// Forward declarations
class Camera;
class Light;
class BufferManager; typedef std::shared_ptr<BufferManager> BufferManagerPtr;
class UniformBufferBlockManager; typedef std::shared_ptr<UniformBufferBlockManager> UniformBufferBlockManagerPtr;

// Main declaration
class LightListUbo {
public:
	LightListUbo(const BufferManagerPtr &buffer_manager, const UniformBufferBlockManagerPtr &ubo_manager);
	~LightListUbo();

	void update(const Camera &active_camera, const std::vector<Light*> &lights);


private:
	static const size_t MAX_LIGHTS = 256;

	struct LightSource {
		glm::vec4 radius; // padded, use only first component
		glm::vec4 position_in_viewspace; // padded, vec3
		glm::vec4 material_color_diffuse; // padded, vec3
		glm::vec4 material_color_specular; // padded, vec3
		glm::vec4 material_color_ambient; // padded vec3
	};

	struct LightList {
		glm::uvec4 light_count; // Padded, use only first component.
		LightSource lights[MAX_LIGHTS];
	};

private:
	Core::BufferAllocation buffer;
};

} // namespace Framework
} // namespace GM

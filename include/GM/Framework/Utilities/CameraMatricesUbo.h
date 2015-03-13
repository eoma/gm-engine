#pragma once

#include "GM/Core/Utilities/BufferAllocation.h"

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

namespace GM {
namespace Framework {

// Forward declarations
class Camera;
class BufferManager; typedef std::shared_ptr<BufferManager> BufferManagerPtr;
class UniformBufferBlockManager; typedef std::shared_ptr<UniformBufferBlockManager> UniformBufferBlockManagerPtr;

// Main declaration
class CameraMatricesUbo {
public:
	CameraMatricesUbo(const BufferManagerPtr &buffer_manager, const UniformBufferBlockManagerPtr &ubo_manager);
	~CameraMatricesUbo();

	void update(const Camera &active_camera);

private:
	struct CameraMatrices {
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
	};

private:
	Core::BufferAllocation buffer;
};

} // namespace Framework
} // namespace GM

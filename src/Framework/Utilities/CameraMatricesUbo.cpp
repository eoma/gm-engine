#include "GM/Framework/Utilities/CameraMatricesUbo.h"

#include "GM/Framework/Components/Camera.h"

#include "GM/Framework/Managers/BufferManager.h"
#include "GM/Framework/Managers/UniformBufferBlockManager.h"

namespace GM {
namespace Framework {

CameraMatricesUbo::CameraMatricesUbo(const BufferManagerPtr &buffer_manager, const UniformBufferBlockManagerPtr &ubo_manager)
{
	int alignment = 0;
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
	buffer = buffer_manager->allocate(sizeof(CameraMatrices), alignment, GL_DYNAMIC_DRAW, BufferManager::UNIQUE_BUFFER, GL_UNIFORM_BUFFER);

	ubo_manager->set_buffer_block("CameraMatrices", buffer);
}

CameraMatricesUbo::~CameraMatricesUbo()
{
}

void CameraMatricesUbo::update(const Camera &active_camera)
{
	buffer.buffer->bind_as(GL_UNIFORM_BUFFER);
	buffer.upload([&active_camera](void* destination, size_t /*max_size*/){
		CameraMatrices *matrices = reinterpret_cast<CameraMatrices*>(destination);
		matrices->ViewMatrix = active_camera.get_view_matrix();
		matrices->ProjectionMatrix = active_camera.get_projection_matrix();
	});
	buffer.buffer->unbind();
}

} // namespace Framework
} // namespace GM

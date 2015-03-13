#include "GM/Framework/Utilities/LightListUbo.h"

#include "GM/Framework/Components/Camera.h"
#include "GM/Framework/Components/Light.h"

#include "GM/Framework/Managers/BufferManager.h"
#include "GM/Framework/Managers/UniformBufferBlockManager.h"

namespace GM {
namespace Framework {

const size_t LightListUbo::MAX_LIGHTS;

LightListUbo::LightListUbo(const BufferManagerPtr &buffer_manager, const UniformBufferBlockManagerPtr &ubo_manager)
{
	int alignment = 0;
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
	buffer = buffer_manager->allocate(sizeof(LightList), alignment, GL_DYNAMIC_DRAW, BufferManager::UNIQUE_BUFFER, GL_UNIFORM_BUFFER);

	ubo_manager->set_buffer_block("LightList", buffer);
}

LightListUbo::~LightListUbo()
{
}

void LightListUbo::update(const Camera &active_camera, const std::vector<Light*> &lights)
{
	buffer.buffer->bind_as(GL_UNIFORM_BUFFER);
	buffer.upload([&active_camera, &lights](void* destination, size_t /*max_size*/){
		LightList *light_list = reinterpret_cast<LightList*>(destination);
		size_t num_lights = std::min(lights.size(), MAX_LIGHTS);

		size_t lightIndex = 0;

		for (size_t i = 0; i < num_lights; ++i)
		{
			const Light *light = lights[i];

			if (light->is_active())
			{
				// Upload light if it is active
				LightSource *light_source = &light_list->lights[lightIndex];

				light_source->radius.x = light->get_radius();
				light_source->position_in_viewspace = glm::vec4(light->get_position_in_viewspace(active_camera), 0);
				light_source->material_color_diffuse = glm::vec4(light->get_material_color_diffuse(), 0);
				light_source->material_color_specular = glm::vec4(light->get_material_color_specular(), 0);
				light_source->material_color_ambient = glm::vec4(light->get_material_color_ambient(), 0);

				++lightIndex;
			}			
		}

		// Update number of light
		light_list->light_count.x = lightIndex;
	});
	buffer.buffer->unbind();
}

} // namespace Framework
} // namespace GM

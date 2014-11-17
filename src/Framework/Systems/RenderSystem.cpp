#include "GM/Framework/Systems/RenderSystem.h"
#include "GM/Framework/Components/Camera.h"
#include "GM/Framework/Components/IRenderable.h"
#include "GM/Framework/Utilities/Material.h"
#include "GM/Framework/Utilities/Mesh.h"

#include "GM/Core/GL/Render.h"
#include "GM/Core/GL/Shader.h"
#include "GM/Core/GL/VertexArrayObject.h"
#include "GM/Core/Utilities/RenderCommand.h"

#include <algorithm>

using namespace GM::Framework;

RenderSystem::RenderSystem() {

}

RenderSystem::~RenderSystem() {

}

void RenderSystem::add_renderable(IRenderable *renderable) {
	unsigned int layer_bits = renderable->get_render_layers();

	for (auto &layer_index : bit_index_maker(layer_bits)) {
		auto &bucket = buckets[layer_index];
		auto iter = std::find(bucket.begin(), bucket.end(), renderable);

		if (iter == bucket.end()) {
			bucket.push_back(renderable);
		}
	}
}

void RenderSystem::remove_renderable(IRenderable *renderable) {
	unsigned int layer_bits = renderable->get_render_layers();

	for (auto &layer_index : bit_index_maker(layer_bits)) {
		auto &bucket = buckets[layer_index];
		auto iter = std::find(bucket.begin(), bucket.end(), renderable);

		if (iter != bucket.end()) {
			bucket.erase(iter);
		}
	}
}

void RenderSystem::add_camera(Camera *camera) {
	unsigned int layer_bits = camera->get_render_layers();

	if (std::find(cameras.begin(), cameras.end(), camera) == cameras.end()) {
		cameras.push_back(camera);
		std::sort(
			cameras.begin(),
			cameras.end(),

			[](const Camera* cam1, const Camera* cam2) -> bool {
				return cam1->get_depth() < cam2->get_depth();
			}
		);

		for (auto &layer_index : bit_index_maker(layer_bits)) {
			auto &depth_sorted_cameras_in_layer = cameras_in_layers[layer_index];

			depth_sorted_cameras_in_layer.push_back(camera);
			std::sort(
				depth_sorted_cameras_in_layer.begin(),
				depth_sorted_cameras_in_layer.end(),

				[](const Camera* cam1, const Camera* cam2) -> bool {
					return cam1->get_depth() < cam2->get_depth();
				}
			);
		}
	}
}

void RenderSystem::remove_camera(Camera *camera) {
	unsigned int layer_bits = camera->get_render_layers();

	for (auto &layer_index : bit_index_maker(layer_bits)) {
		auto &depth_sorted_cameras = cameras_in_layers[layer_index];
		auto iter = std::find(depth_sorted_cameras.begin(), depth_sorted_cameras.end(), camera);

		if (iter != depth_sorted_cameras.end()) {
			depth_sorted_cameras.erase(iter);
		}
	}
}

void RenderSystem::add_light(Light *light) {
	auto iter = std::find(lights.begin(), lights.end(), light);
	if (iter == lights.end()) {
		lights.push_back(light);
	}
}

void RenderSystem::remove_light(Light *light) {
	auto iter = std::find(lights.begin(), lights.end(), light);
	if (iter == lights.end()) {
		lights.erase(iter);
	}
}

std::vector<unsigned int> RenderSystem::bit_index_maker(const unsigned int bits) {
	typedef std::numeric_limits<unsigned int> UnsignedIntLimits;

	std::vector<unsigned int> indices;
	indices.reserve(UnsignedIntLimits::digits);

	for (unsigned int index = 0; index < UnsignedIntLimits::digits; ++index) {
		if (((1 << index) & bits) > 0) {
			indices.push_back(index);
		}
	}

	return indices;
}

const std::vector<IRenderable*> &RenderSystem::get_bucket(const unsigned int bucket_index) {
	return buckets.at(bucket_index);
}

const std::vector<Camera*> &RenderSystem::get_cameras(const unsigned int layer_index) {
	return cameras_in_layers.at(layer_index);
}

void RenderSystem::render() {

	MaterialPtr active_material = nullptr;
	MeshPtr active_mesh = nullptr;

	Core::VertexArrayObjectPtr active_vao = nullptr;
	Core::ShaderPtr active_shader = nullptr;

	for (unsigned int layer = 0; layer < buckets.size(); ++layer) {
		const auto &bucket = buckets[layer];
		const auto &cameras = cameras_in_layers[layer];

		Core::RenderCommand command;

		// Draw from depth zero and up
		for (Camera *camera : cameras)	{
			const std::string render_pass_name = "standard";

			camera->clear_buffer();

			for (IRenderable *renderable : bucket) {
				if (active_mesh != renderable->get_mesh())
				{
					active_mesh = renderable->get_mesh();

					if (active_mesh == nullptr)
					{
						continue; // Jump to next renderable
					}

					if (active_vao != active_mesh->get_vao())
					{
						active_vao = active_mesh->get_vao();
						active_vao->bind();
					}

					command = active_mesh->get_render_command();
				}

				if (active_material != renderable->get_material())
				{
					// unbind previous?
					active_material = renderable->get_material();

					if (active_material == nullptr)
					{
						continue; // Jump to next renderable
					}

					// Get appropriate shader type, depending pass
					if (active_shader != active_material->get_render_pass(render_pass_name))
					{
						active_shader = active_material->get_render_pass(render_pass_name);
						active_shader->bind();
						// Update camera uniforms?
					}

					if (active_shader == nullptr)
					{
						continue;
					}

					// FIXME: bind textures every frame?
					active_material->update_uniforms(camera, lights, render_pass_name);
				}

				renderable->update_uniforms(camera, render_pass_name);

				if (renderable->has_custom_render()) {
					renderable->custom_render(camera);
				}
				else
				{
					Core::Render::render(command);
				}
			}
		}
	}

	if (active_shader != nullptr)
	{
		active_shader->unbind();
	}

	if (active_vao != nullptr)
	{
		active_vao->unbind();
	}

	// Perform post-process?
}

void RenderSystem::resize(int width, int height) {
	// FIXME: Should move this to Core::ViewportManager, that holds a push/pop functionality for pushing viewport sizes. 
	//        Very useful when working with framebuffers that holds different buffer/viewport sizes.
	glViewport(0, 0, width, height);

	// FIXME: Cameras should probably be stored in a pure list as well as in buckets, so that we can iterate over them
	//        without risking that a camera's projection is calculated more than once.
	for (unsigned int layer = 0; layer < buckets.size(); ++layer) {
		const auto &cameras = cameras_in_layers[layer];
		for (Camera *camera : cameras)	{
			camera->set_projection(width, height);
		}
	}
}

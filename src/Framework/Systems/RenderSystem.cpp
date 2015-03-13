#include "GM/Framework/Systems/RenderSystem.h"
#include "GM/Framework/Components/Camera.h"
#include "GM/Framework/Components/Light.h"
#include "GM/Framework/Components/IRenderable.h"
#include "GM/Framework/Components/IRenderPassComponent.h"
#include "GM/Framework/Utilities/Material.h"
#include "GM/Framework/Utilities/Mesh.h"
#include "GM/Framework/Utilities/CameraMatricesUbo.h"
#include "GM/Framework/Utilities/LightListUbo.h"

#include "GM/Core/GL/FramebufferObject.h"
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

		for (auto &layer_index : bit_index_maker(layer_bits)) {
			auto &depth_sorted_cameras_in_layer = cameras_in_layers[layer_index];

			depth_sorted_cameras_in_layer.push_back(camera);
		}
	}

	sort_cameras();
}

void RenderSystem::remove_camera(Camera *camera) {
	unsigned int layer_bits = camera->get_render_layers();

	auto iter = std::find(cameras.begin(), cameras.end(), camera);
	if (iter != cameras.end()) {
		cameras.erase(iter);

		for (auto &layer_index : bit_index_maker(layer_bits)) {
			auto &depth_sorted_cameras = cameras_in_layers[layer_index];
			iter = std::find(depth_sorted_cameras.begin(), depth_sorted_cameras.end(), camera);

			if (iter != depth_sorted_cameras.end()) {
				depth_sorted_cameras.erase(iter);
			}
		}
	}
}

void RenderSystem::sort_cameras() {
	std::sort(
		cameras.begin(),
		cameras.end(),

		[](const Camera* cam1, const Camera* cam2) -> bool {
			return cam1->get_depth() < cam2->get_depth();
		}
	);

	for (auto &depth_sorted_cameras_in_layer : cameras_in_layers) {
		std::sort(
			depth_sorted_cameras_in_layer.begin(),
			depth_sorted_cameras_in_layer.end(),

			[](const Camera* cam1, const Camera* cam2) -> bool {
				return cam1->get_depth() < cam2->get_depth();
			}
		);
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

	// Remember that cameras are depth sorted, start at minimal depth and go upwards
	for (Camera *cam : cameras)
	{
		prepare_ubos(*cam);
		for (IRenderPassComponent *render_pass : cam->get_render_pass_sequence())
		{
			render_pass->pass(*this);
		}
	}

}

// Called by render passes
void RenderSystem::pass(Camera &camera, const std::string &render_pass_name, unsigned int accepted_layers)
{

	MaterialPtr active_material = nullptr;
	MeshPtr active_mesh = nullptr;

	Core::VertexArrayObjectPtr active_vao = nullptr;
	Core::ShaderPtr active_shader = nullptr;

	for (unsigned int layer : bit_index_maker(camera.get_render_layers() & accepted_layers))
	{
		const auto& bucket = buckets[layer];

		for (IRenderable *renderable : bucket)
		{
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
			}
			else if (active_mesh == nullptr)
			{
				// throw?
				continue;
			}

			if (active_material != renderable->get_material())
			{
				// unbind previous?
				active_material = renderable->get_material();

				if (active_material == nullptr || !active_material->has_render_pass(render_pass_name))
				{
					continue; // Jump to next renderable
				}

				// Get appropriate shader type, depending pass
				if (active_shader != active_material->get_render_pass(render_pass_name))
				{
					active_shader = active_material->get_render_pass(render_pass_name);
					if (active_shader == nullptr)
					{
						continue;
					}

					active_shader->bind();
				} else if (active_shader == nullptr) {
					continue;
				}

				// FIXME: bind textures every frame?
				active_material->update_uniforms(camera, lights, render_pass_name);
			}
			else if (active_material == nullptr)
			{
				continue;
			}

			renderable->update_uniforms(camera, render_pass_name);

			if (renderable->has_custom_render()) {
				renderable->custom_render(camera);
			}
			else
			{
				Core::Render::render(active_mesh->get_render_command());
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
}

void RenderSystem::resize(int width, int height)
{
	// FIXME: Should move this to Core::ViewportManager, that holds a push/pop functionality for pushing viewport sizes. 
	//        Very useful when working with framebuffers that holds different buffer/viewport sizes.
	glViewport(0, 0, width, height);

	for (Camera *cam : cameras)
	{
		cam->set_projection(width, height);
	}
}

void RenderSystem::prepare_ubos(const Camera &active_camera)
{
	if (camera_matrices_ubo != nullptr)
	{
		camera_matrices_ubo->update(active_camera);
	}

	if (light_list_ubo != nullptr)
	{
		light_list_ubo->update(active_camera, lights);
	}
}

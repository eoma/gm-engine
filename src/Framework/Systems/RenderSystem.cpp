
#include "GM/Framework/Components/Camera.h"
#include "GM/Framework/Components/IRenderable.h"
#include "GM/Framework/Systems/RenderSystem.h"

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

	for (auto &layer_index : bit_index_maker(layer_bits)) {
		auto &depth_sorted_cameras = cameras_in_layers[layer_index];
		auto iter = std::find(depth_sorted_cameras.begin(), depth_sorted_cameras.end(), camera);

		if (iter == depth_sorted_cameras.end()) {
			depth_sorted_cameras.push_back(camera);
			std::sort(
				depth_sorted_cameras.begin(),
				depth_sorted_cameras.end(),

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
	if (bucket_index < std::numeric_limits<unsigned int>::digits) {
		return buckets[bucket_index];
	} else {
		throw std::runtime_error("index out of bounds");
	}
}

void RenderSystem::render() {
	for (unsigned int layer = 0; layer < buckets.size(); ++layer) {
		const auto &bucket = buckets[layer];
		const auto &cameras = cameras_in_layers[layer];

		// Draw from depth zero and up
		for (Camera *camera : cameras)	{
			for (IRenderable *renderable : bucket) {
				renderable->render(camera);
			}
		}
	}
}

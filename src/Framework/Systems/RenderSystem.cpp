#include "GM/Framework/Systems/RenderSystem.h"

#include <algorithm>

using namespace GM::Framework;

RenderSystem::RenderSystem() {

}

RenderSystem::~RenderSystem() {

}

void RenderSystem::add_renderable(const IRenderable *renderable) {
	auto iter = std::find(renderables.begin(), renderables.end(), renderable);

	if (iter == renderables.end()) {
		renderables.push_back(renderable);
	}
}

void RenderSystem::remove_renderable(const IRenderable *renderable) {
	auto iter = std::find(renderables.begin(), renderables.end(), renderable);

	if (iter != renderables.end()) {
		renderables.erase(iter);
	}
}

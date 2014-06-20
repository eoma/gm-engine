#include "GM/Framework/Systems/RenderSystem.h"

using namespace GM::Framework;

RenderSystem::RenderSystem() {

}

RenderSystem::~RenderSystem() {

}

void RenderSystem::add_renderable(const IRenderable *renderable) {
	renderables.push_back(renderable);
}

void RenderSystem::remove_renderable(const IRenderable *renderable) {
	renderables.push_back(renderable);
}

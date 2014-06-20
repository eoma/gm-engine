#pragma once

#include <memory>
#include <vector>


namespace GM {
namespace Framework {

class IRenderable; typedef std::shared_ptr<IRenderable> IRenderablePtr;
class RenderSystem; typedef std::shared_ptr<RenderSystem> RenderSystemPtr;

class RenderSystem {
public:
	RenderSystem();
	~RenderSystem();

	void add_renderable(const IRenderable *renderable);
	void remove_renderable(const IRenderable *renderable);

private:
	std::vector<const IRenderable*> renderables;
};

} // namespace Framework
} // namespace GM

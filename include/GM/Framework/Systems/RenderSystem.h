#pragma once

#include <array>
#include <limits>
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

	void add_renderable(IRenderable *renderable);
	void remove_renderable(IRenderable *renderable);

	void render();

	const std::vector<IRenderable*> &get_bucket(const unsigned int bucket_index);

public:

	// Should this be moved to a utility class?
	static std::vector<unsigned int> bit_index_maker(const unsigned int bits);

private:
	std::array<std::vector<IRenderable*>, std::numeric_limits<unsigned int>::digits> buckets;
};

} // namespace Framework
} // namespace GM

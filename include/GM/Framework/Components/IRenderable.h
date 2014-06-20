#pragma once

#include <memory>

namespace GM {
namespace Framework {

class IRenderable; typedef std::shared_ptr<IRenderable> IRenderablePtr;

class IRenderable {
public:
	virtual ~IRenderable() = 0;

};

} // namespace Framework
} // namespace GM

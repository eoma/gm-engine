#pragma once

#include <memory>

namespace GM {
namespace Framework {

	class Material; typedef std::shared_ptr<Material> MaterialPtr;

class MaterialManager {
public:
	MaterialManager();
	~MaterialManager();

public:

private:
};

typedef std::shared_ptr<MaterialManager> MaterialManagerPtr;

} // namespace Framework
} // namespace GM

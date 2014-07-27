#pragma once

#include <memory>

namespace GM {
namespace Framework {

	class Material; typedef std::shared_ptr<Material> MaterialPtr;

class MaterialSystem {
public:
	MaterialSystem();
	~MaterialSystem();

public:

private:
};

typedef std::shared_ptr<MaterialSystem> MaterialSystemPtr;

} // namespace Framework
} // namespace GM

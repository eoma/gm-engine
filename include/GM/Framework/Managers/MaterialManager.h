#pragma once

#include <map>
#include <memory>
#include <string>

namespace GM {
namespace Framework {

	class Material; typedef std::shared_ptr<Material> MaterialPtr;

class MaterialManager {
public:
	MaterialManager();
	~MaterialManager();

	bool contains(const std::string &name) const;

	void add(const std::string& name, const MaterialPtr &material);

	MaterialPtr get(const std::string& name) const;

	//MaterialPtr get_or_create(const std::string& name);
public:

private:
	std::map<std::string, MaterialPtr> materials;
};

typedef std::shared_ptr<MaterialManager> MaterialManagerPtr;

} // namespace Framework
} // namespace GM

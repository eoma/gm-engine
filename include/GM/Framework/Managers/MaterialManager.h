#pragma once

#include <map>
#include <memory>
#include <string>

namespace GM {
namespace Framework {

	class Material; typedef std::shared_ptr<Material> MaterialPtr;
	class MaterialTemplateManager; typedef std::shared_ptr<MaterialTemplateManager> MaterialTemplateManagerPtr;
	class ShaderManager; typedef std::shared_ptr<ShaderManager> ShaderManagerPtr;
	class TextureManager; typedef std::shared_ptr<TextureManager> TextureManagerPtr;

class MaterialManager {
public:
	MaterialManager(const ShaderManagerPtr &shader_manager, const TextureManagerPtr &texture_manager);
	~MaterialManager();

	bool contains(const std::string &name) const;

	void add(const std::string& name, const MaterialPtr &material);

	MaterialPtr get(const std::string& name) const;
	MaterialPtr get_or_create(const std::string& name);
	MaterialPtr get_or_create(const std::string& name, const std::string& shader_name);

	void add_templates(const std::string &template_filename);
public:

private:
	std::map<std::string, MaterialPtr> materials;

	MaterialTemplateManagerPtr template_manager;
	ShaderManagerPtr shader_manager;
	TextureManagerPtr texture_manager;
};

typedef std::shared_ptr<MaterialManager> MaterialManagerPtr;

} // namespace Framework
} // namespace GM

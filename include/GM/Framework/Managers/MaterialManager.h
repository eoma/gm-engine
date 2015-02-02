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
	MaterialManager(const ShaderManagerPtr &shader_manager,
	                const TextureManagerPtr &texture_manager);
	~MaterialManager();

	bool contains(const std::string &name) const;

	void add(const MaterialPtr &material);

	/**
	 * Gets and returns a shared pointer to an already defined material
	 * or return a nullptr if the named material does not exist
	 */
	MaterialPtr get(const std::string& name) const;

	/**
	 * Creates a material based on a template. Will in essence call get_or_create() with a map
	 * of shader pass associations and insert the properties defined in the template.
	 */
	MaterialPtr get_or_create(const std::string& name);

	/**
	 * Gets or creates (if non-existing) a new material with a single 
	 * shader associated to the standard pass.
	 */
	MaterialPtr get_or_create(const std::string& name, const std::string& shader_name);

	/*
	 * Gets or creates (if non-existing) a new material with a map of shader pass associations
	 */
	MaterialPtr get_or_create(const std::string& name, const std::map<std::string, 
	                          std::string> &pass_shader_name_map);

	/**
	 * Read template definitions from the given file
	 */
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

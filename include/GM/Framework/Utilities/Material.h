#pragma once

#include "../Totem/PropertyContainer.h"

#include "../../Core/GL/Shader.h"
#include "../../Core/Utilities/UniformOperations.h"
#include "PropertyToUniformConnector.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

namespace GM {

namespace Core {
	class Shader; typedef std::shared_ptr<Shader> ShaderPtr;
	class Texture; typedef std::shared_ptr<Texture> TexturePtr;
}

namespace Framework {

class IProperty; typedef std::shared_ptr<IProperty> IPropertyPtr;
class TextureManager; typedef std::shared_ptr<TextureManager> TextureManagerPtr;
class Camera;
class Light;
class Tessellator;
class PropertyToUniformConnector;

class Material : public PropertyContainer<>
{
public:
	Material(const TextureManagerPtr &texture_manager, const Core::ShaderPtr &standard_render_pass_shader, const std::string &name);
	Material(const TextureManagerPtr &texture_manager, const std::map<std::string, Core::ShaderPtr> &pass_shaders, const std::string &name);
	virtual ~Material() {};

	const std::string &get_name() const { return name; }

	void update_uniforms(const Camera &camera, const std::vector<Light *> &lights, const std::string &render_pass_name = "standard");

	// Methods for testing, fetching and setting render pass shaders
	bool has_render_pass(const std::string &render_pass_name = "standard") const;
	const Core::ShaderPtr &get_render_pass(const std::string &render_pass_name = "standard") const;
	void set_render_pass(const Core::ShaderPtr &shader, const std::string &render_pass_name = "standard");

	// Specific render pass configurations, useful for renderable
	const PropertyToUniformConnector &get_render_pass_config(const std::string &render_pass_name) const;
	const std::map<std::string, PropertyToUniformConnector> &get_render_pass_configs() const;

private:
	std::string name;

	TextureManagerPtr texture_manager;

	// Render pass shader specific data
	std::map<std::string, PropertyToUniformConnector> render_pass_configs;

	// All uniforms directly used by this material
	std::vector<std::string> used_uniforms;

	Property<int> patch_vertices_property;

	clan::Slot property_added_slot;
};

//
// Implementations
//

} // namespace Framework
} // namespace GM

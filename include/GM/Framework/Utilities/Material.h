#pragma once

#include "../Totem/PropertyContainer.h"

#include "../../Core/GL/Shader.h"
#include "../../Core/Utilities/UniformOperations.h"

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

class Material : public PropertyContainer<>
{
public:
	struct RenderPassConfig {
		RenderPassConfig(const Core::ShaderPtr &shader = nullptr) : shader(shader), current_available_texture_unit(0) {}

		int compute_available_texture_unit() { return current_available_texture_unit++; }

		//

		Core::ShaderPtr shader;

		int current_available_texture_unit;

		clan::Signal<void()> update_uniforms_signal;
		clan::SlotContainer update_uniform_slots;

		// Uniforms directly used by this material
		std::vector<std::string> used_uniforms;

		// Uniforms _not_ used by this material, useful for entities referencing this material
		std::vector<std::string> unused_uniforms;
		std::map<std::string, clan::Slot> wait_for_first_write_slots;
	};

public:
	Material(const TextureManagerPtr &texture_manager, const Core::ShaderPtr &standard_render_pass_shader, const std::string &name);
	virtual ~Material() {};

	const std::string &get_name() const { return name; }

	void update_uniforms(Camera * camera, const std::vector<Light *> &lights, const std::string &render_pass_name = "standard");

	// Methods for testing, fetching and setting render pass shaders
	bool has_render_pass(const std::string &render_pass_name = "standard") const;
	const Core::ShaderPtr &get_render_pass(const std::string &render_pass_name = "standard") const;
	void set_render_pass(const Core::ShaderPtr &shader, const std::string &render_pass_name = "standard");

	// Specific render pass configurations, useful for renderable
	const Material::RenderPassConfig &get_render_pass_config(const std::string &render_pass_name) const;
	const std::map<std::string, Material::RenderPassConfig> &get_render_pass_configs() const;

private:
	void set_up_uniforms(const std::string &render_pass_name);

	template<class T>
	void add_sleeping_uniform(const std::string &render_pass_name, const std::string &name, const T &default_value, const unsigned int program, const int uniform_location);

	template<class T>
	std::function<void()> uniform_lambda_maker(const std::string &render_pass_name, const unsigned int program, const int uniform_location, const Property<T> &prop);

private:
	std::string name;

	TextureManagerPtr texture_manager;

	// Render pass shader specific data
	std::map<std::string, Material::RenderPassConfig> render_pass_configs;

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

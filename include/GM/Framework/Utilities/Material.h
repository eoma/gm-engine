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

class Material : public PropertyContainer<>
{
public:
	Material(const TextureManagerPtr &texture_manager, const Core::ShaderPtr &shader, const std::string &name = std::string());
	virtual ~Material() {};

	const std::string &get_name() const { return name; }
	Core::ShaderPtr &get_shader() { return shader; }

	void update_uniforms() { update_uniforms_signal(); }

	const std::vector<std::string> &get_unused_uniforms() const { return unused_uniforms; }

private:
	// Probably need one specifically made for textures
	template<class T>
	void add_sleeping_uniform(const std::string &name, const T &default_value, const unsigned int program, const int uniform_location);

private:
	std::string name;
	Core::ShaderPtr shader;
	TextureManagerPtr texture_manager;

	// Uniforms directly used by this material
	std::vector<std::string> used_uniforms;

	// Uniforms _not_ used by this material
	std::vector<std::string> unused_uniforms;
	std::map<std::string, clan::Slot> unused_first_write_slots;

	clan::Signal<void()> update_uniforms_signal;
	clan::SlotContainer update_uniform_slots;
	clan::Slot property_added_slot;
};

//
// Implementations
//

template <class T>
void Material::add_sleeping_uniform(const std::string &name, const T &default_value, const unsigned int program, const int uniform_location)
{
	auto prop = add<T>(name, default_value);

	auto update_lambda = [program, uniform_location, prop]() {
		Core::update_uniform(program, uniform_location, (T)prop.get());
	};

	unused_first_write_slots.emplace(
		name,
		prop.value_changed().connect([this, name, update_lambda](const T &/*old_v*/, const T &/*new_v*/) {
			unused_uniforms.erase(std::find(unused_uniforms.begin(), unused_uniforms.end(), name));
			unused_first_write_slots.erase(name);
			used_uniforms.push_back(name);

			update_uniform_slots.connect(update_uniforms_signal, update_lambda);
		})
	);
}

} // namespace Framework
} // namespace GM

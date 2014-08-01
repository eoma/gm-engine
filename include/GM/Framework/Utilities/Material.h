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


class Material : public PropertyContainer<>
{
public:
	Material(const Core::ShaderPtr &shader, const std::string &name = std::string());
	virtual ~Material() {};

	const std::string &get_name() const { return name; }
	Core::ShaderPtr &get_shader() { return shader; }

	template<class T>
	void add_uniform(const std::string &name, const T &default_value);

	// Convenience
	void add_texture(const std::string &name, const Core::TexturePtr &value);

	void update_uniforms() { update_uniforms_signal(); }

	const std::vector<std::string> &get_unused_uniforms() const { return unused_uniforms; }

private:
	template<class T>
	void add_uniform(const std::string &name, const T &default_value, const unsigned int program, const int uniform_location);

private:
	std::string name;
	Core::ShaderPtr shader;

	// Uniforms directly used by this material
	std::vector<std::string> used_uniforms;

	// Uniforms _not_ used by this material
	std::vector<std::string> unused_uniforms;

	clan::Signal<void()> update_uniforms_signal;
	clan::SlotContainer update_uniform_slots;
	clan::Slot property_added_slot;
};

//
// Implementations
//

template <class T>
void Material::add_uniform(const std::string &name, const T &default_value)
{
	// FIXME: Had to comment out this and add a local listener to property added signal in .cpp to get json settings to apply to uniforms...

	//if (!has_property(name)) {
		if (!shader->contains_uniform(name))
		{
			std::cout << "Shader does not contain uniform " << name << std::endl;
			return;
		}

		Core::ShaderVariableInfo info = shader->get_uniform_info(name);
		add_uniform<T>(name, default_value, shader->get_handle(), info.location);
	/*}
	else
	{
		std::cout << "Material already contains uniform " << name << std::endl;
	}*/
}

template <class T>
void Material::add_uniform(const std::string &name, const T &default_value, const unsigned int program, const int uniform_location)
{
	auto prop = add<T>(name, default_value);

	update_uniform_slots.connect(update_uniforms_signal, [program, uniform_location, prop]() {
		Core::update_uniform(program, uniform_location, (T)prop.get());
	});

	auto found_uniform = std::find(unused_uniforms.begin(), unused_uniforms.end(), name);
	used_uniforms.push_back(*found_uniform);
	unused_uniforms.erase(found_uniform);

	//property_uniform_pairs.push_back(std::make_pair(prop, uniform_location));
}

} // namespace Framework
} // namespace GM

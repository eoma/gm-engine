#pragma once

#include "../Totem/PropertyContainer.h"

#include <memory>

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

	// Care must be used when updating textures
	const std::vector<std::pair<IPropertyPtr, int>> &get_property_uniform_pairs() const { return property_uniform_pairs; };

	template<class T>
	void add_uniform(const std::string &name, const T &default_value);

	// Convenience
	void add_texture(const std::string &name, const Core::TexturePtr &value);

private:
	template<class T>
	void add_uniform(const std::string &name, const T &default_value, int uniform_location);

private:
	std::string name;
	Core::ShaderPtr shader;

	// Stores pairs of properties and the corresponding uniform location
	std::vector<std::pair<IPropertyPtr, int>> property_uniform_pairs;
};

//
// Implementations
//

template <class T>
void Material::add_uniform(const std::string &name, const T &default_value)
{
	// FIXME: query shader for name, type, fetch uniform location
	//

	if (!has_property(name)) {
		int uniform_location = 0;
		add_uniform<T>(name, default_value, uniform_location);
	}
}

template <class T>
void Material::add_uniform(const std::string &name, const T &default_value, const int uniform_location)
{
	auto prop = std::make_shared<Property<T>>(add<T>(name, default_value));

	property_uniform_pairs.push_back(std::make_pair(prop, uniform_location));
}

} // namespace Framework
} // namespace GM

#pragma once

#include "../DefinitionsComponentNames.h"
#include "../Entity.h"
#include "../Totem/Component.h"

#include <memory>

namespace GM {

namespace Core {
	class Shader; typedef std::shared_ptr<Shader> ShaderPtr;
	class Texture; typedef std::shared_ptr<Texture> TexturePtr;
}

namespace Framework {

class IProperty; typedef std::shared_ptr<IProperty> IPropertyPtr;


class Material : public Component<Material>
{
public:
	Material(const EntityPtr &owner, const Core::ShaderPtr &shader, const std::string &name = std::string());
	virtual ~Material() {};

	std::string get_type() const override { return get_static_type(); };

	Core::ShaderPtr &get_shader() { return shader; }

	// Care must be used when updating textures
	const std::vector<std::pair<IPropertyPtr, unsigned int>> &get_property_uniform_pairs() const { return property_uniform_pairs; };

	template<class T>
	void add_uniform(const std::string &name, const T &default_value);

	// Convenience
	void add_texture(const std::string &name, const Core::TexturePtr &value);

protected:
	Core::ShaderPtr shader;

	// Stores pairs of properties and the corresponding uniform location
	std::vector<std::pair<IPropertyPtr, unsigned int>> property_uniform_pairs;

private:
	static std::string get_static_type() { return COMPONENT_MATERIAL; };
};

template <class T>
void Material::add_uniform(const std::string &name, const T &default_value)
{
	// query shader for name, type, fetch uniform location
	//
	unsigned int uniform_location = 0;

	auto prop = std::make_shared<Property<T>>(owner->add<T>(name, default_value));

	property_uniform_pairs.push_back(std::make_pair(prop, uniform_location));
}

} // namespace Framework
} // namespace GM

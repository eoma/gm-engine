#pragma once

#include <Totem/ComponentContainer.h>
#include <Totem/PropertyContainer.h>

#include <string>

namespace GM {
namespace Framework {

class Entity : public Totem::PropertyContainer<>, public Totem::ComponentContainer<> {
public:
	Entity(const std::string &name);
	virtual ~Entity() {};

	std::string get_name() const { return name; };

private:
	std::string name;
};

} // namespace Framework
} // namespace GM

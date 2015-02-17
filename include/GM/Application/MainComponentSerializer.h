#pragma once

#include <ClanLib/core.h>

#include <memory>
#include <string>
#include <unordered_map>

namespace GM {

namespace Framework {
	class Entity; typedef std::shared_ptr<Entity> EntityPtr;
}

namespace Application {

class Main; typedef std::shared_ptr<Main> MainPtr;

/**
 * The MainComponentSerializer class is used in conjunction with the template
 * managers for creating the components specified in the template.
 *
 * On construction, it will fetch Main's entity manager and connect
 * create_and_add_component() to the entity manager's register_component_serializer_signal.
 *
 * It is possible to add custom component creators through the function
 * add_component_creator().
 *
 * This class should be owned by the Main class.
 */
class MainComponentSerializer {
public:
	MainComponentSerializer(Main *app);

	/**
	 * Will attempt to create and add the component registered by
	 * the name of type to the entity owner.
	 *
	 * @param owner the entity that will own the component
	 * @param component_type_name the type name of the component
	 * @param name the component name
	 */
	void create_and_add_component(const Framework::EntityPtr &owner, const std::string &component_type_name, const std::string &name);

	/**
	 * Adds a component creator function for the component type named component_type_name
	 *
	 * @param component_type_name the name of this component's type. Often given as MyComponent::get_static_type()
	 * @param creator_function a lambda function (or functor) that will create the component.
	 */
	void add_component_creator(const std::string &component_type_name, std::function<void(const Framework::EntityPtr &)> &&creator_function);

private:
	Main *app;
	clan::SlotContainer slots;
	std::unordered_map<std::string, std::function<void(const Framework::EntityPtr &owner)>> component_creators;
};
typedef std::shared_ptr<MainComponentSerializer> MainComponentSerializerPtr;

} // namespace Application
} // namespace GM

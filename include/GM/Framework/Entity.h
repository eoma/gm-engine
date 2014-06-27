#pragma once

#include "Totem/Totem.h"

#include <atomic>
#include <memory>
#include <string>
#include <utility>

namespace GM {
namespace Framework {

class Transform; typedef std::shared_ptr<Transform> TransformPtr;

class Entity : public PropertyContainer<>, public ComponentContainer<>, public std::enable_shared_from_this<Entity> {
public:
	Entity(const std::string &name);
	virtual ~Entity();

	std::string get_name() const { return name; };

	TransformPtr get_transform(std::function<void(const TransformPtr &)> func = nullptr) const;
	bool has_transform() const { return transform != nullptr; }

	unsigned long get_id() const { return id; };


	/**
	 * Convenience template method to create a component and connect
	 * it immediately to the entity. The relevant component _must_ have
	 * the entity owner (of type EntityPtr) as first parameter.
	 *
	 * To call this method with, say, Transform, just call 
	 * create_component<Transform>(scene_manager).
	 *
	 * You can still add components in the old way, for example 
	 * entity->add_component<Transform>(TransformPtr(new Transform(entity, scene_manager)).
	 *
	 * This method can not be used within Entity's constructor!
	 */
	template<class ComponentType, typename... Params>
	std::shared_ptr<ComponentType> create_component(Params&&... parameters) {
		return this->add_component<ComponentType>(
			std::make_shared<ComponentType>(shared_from_this(), std::forward<Params>(parameters)...)
		);
	}

private:
	void on_component_added(std::shared_ptr<IComponent<>> component);
	void on_component_removed(std::shared_ptr<IComponent<>> component);

private:
	static std::atomic<unsigned long> next_id;

private:
	const unsigned long id;
	std::string name;

	// It's needed to save connected callbacks in order to disconnect them
	clan::Callback<void(std::shared_ptr<IComponent<>>)> component_removed_slot;

	TransformPtr transform;
};

} // namespace Framework
} // namespace GM

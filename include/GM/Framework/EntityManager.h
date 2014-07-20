#pragma once

#include <ClanLib/core.h>

#include <memory>
#include <vector>

namespace GM {
namespace Framework {

class ComponentSerializer; typedef std::shared_ptr<ComponentSerializer> ComponentSerializerPtr;
class Entity; typedef std::shared_ptr<Entity> EntityPtr;
class EntityTemplateManager; typedef std::shared_ptr<EntityTemplateManager> EntityTemplateManagerPtr;

class EntityManager {
public:
	EntityManager();
	~EntityManager();

	EntityPtr get_entity(const std::string &name) const;
	const std::vector<EntityPtr> &get_entities() const;

	EntityPtr create_entity(const std::string &name);
	EntityPtr create_entity(const std::string &name, const std::string &template_name);
	EntityPtr remove_entity(const std::string &name, bool immediate = false);

	EntityPtr add_entity(EntityPtr entity);
	EntityPtr remove_entity(const EntityPtr &entity, bool immediate = false);
	EntityPtr remove_entity(const unsigned long id, bool immediate = false);

	void update(float elapsed_time);

	//ComponentSerializerPtr get_component_serializer() const { return component_serializer; }
	clan::Signal<void(const EntityPtr &/*owner*/, const std::string &/*type*/, const std::string &/*name*/)> register_component_serializer_signal();

	void add_templates(const std::string template_filename);
	void apply(const std::string &template_name, const EntityPtr &entity);

private:
	std::vector<EntityPtr> entities;
	std::vector<EntityPtr> pending_deletion;

	ComponentSerializerPtr component_serializer;
	EntityTemplateManagerPtr template_manager;
};

typedef std::shared_ptr<EntityManager> EntityManagerPtr;

} // namespace Framework
} // namespace GM

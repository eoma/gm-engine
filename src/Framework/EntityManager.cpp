#include "GM/Framework/EntityManager.h"

#include "GM/Framework/Entity.h"

#include <algorithm>
#include <iostream>

using namespace GM::Framework;

EntityManager::EntityManager()
: entities()
, pending_deletion()
{
}

EntityManager::~EntityManager()
{
	//std::cout << "EntityManager destroyed" << std::endl;
}

void EntityManager::update(float elapsed_time)
{
	for(auto entity : pending_deletion)
	{
		std::cout << "Removing " + entity->get_name() << std::endl;
		remove_entity(entity, true);
	}
	pending_deletion.clear();

	for (auto entity : entities)
	{
		entity->update_components(elapsed_time);
		entity->update_properties();
	}
}

EntityPtr EntityManager::get_entity(const std::string &name) const
{
	for(auto entity : entities)
	{
		if(entity->get_name() == name)
			return entity;
	}

	return nullptr;
}

const std::vector<EntityPtr> &EntityManager::get_entities() const
{
	return entities;
}

EntityPtr EntityManager::create_entity(const std::string &name)
{
	auto entity = EntityPtr(new Entity(name));
	entities.push_back(entity);
	return entity;
}

EntityPtr EntityManager::remove_entity(const std::string &name, bool immediate)
{
	for(unsigned int i = 0; i < entities.size(); i++)
	{
		auto entity = entities[i];
		if(entity->get_name() == name)
		{
			if(immediate)
			{
				//entities.erase(entities.begin() + i);
				entities[i] = entities.back();
				entities.pop_back();
			}
			else
			{
				pending_deletion.push_back(entity);
			}
			return entity;
		}
	}

	return nullptr;
}

EntityPtr EntityManager::add_entity(EntityPtr entity)
{
	entities.push_back(entity);
	return entity;
}

EntityPtr EntityManager::remove_entity(const EntityPtr &entity, bool immediate)
{
	auto entity_it = std::find(entities.begin(), entities.end(), entity);
	if(entity_it != entities.end())
	{
		if(immediate)
			entities.erase(entity_it);
		else
			pending_deletion.push_back(entity);
	}

	return entity;
}

EntityPtr EntityManager::remove_entity(const unsigned long id, bool immediate) {
	// Can use binary search (using std::lower_bound) if we can guarantee that
	// the entity manager is the only one to create entities.
	auto entity_it = std::find_if(entities.begin(), entities.end(), 
		[id](const EntityPtr &entity) { return entity->get_id() == id; });

	EntityPtr entity = nullptr;

	if(entity_it != entities.end())
	{
		entity = *entity_it;
		if(immediate)
			entities.erase(entity_it);
		else
			pending_deletion.push_back(entity);
	}

	return entity;
}

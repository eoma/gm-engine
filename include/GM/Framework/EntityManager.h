#pragma once

#include <memory>
#include <vector>

namespace GM {
namespace Framework {

class Entity; typedef std::shared_ptr<Entity> EntityPtr;

class EntityManager {
public:
	EntityManager();
	~EntityManager();

	EntityPtr get_entity(const std::string &name) const;
	const std::vector<EntityPtr> &get_entities() const;

	EntityPtr create_entity(const std::string &name);
	EntityPtr remove_entity(const std::string &name, bool immediate = false);

	EntityPtr add_entity(EntityPtr entity);
	EntityPtr remove_entity(const EntityPtr &entity, bool immediate = false);
	EntityPtr remove_entity(const unsigned long id, bool immediate = false);

	void update();

private:
	std::vector<EntityPtr> entities;
	std::vector<EntityPtr> pending_deletion;
};

typedef std::shared_ptr<EntityManager> EntityManagerPtr;

} // namespace Framework
} // namespace GM

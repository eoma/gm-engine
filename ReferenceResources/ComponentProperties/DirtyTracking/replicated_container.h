#pragma once

class ServerGameObject; typedef std::shared_ptr<ServerGameObject> ServerGameObjectPtr;
class ComponentSerializer; typedef std::shared_ptr<ComponentSerializer> ComponentSerializerPtr;
class Player;

class ReplicatedContainer
{
// Construction:
public:
	ReplicatedContainer(Player *player, const ComponentSerializerPtr &component_serializer);
	~ReplicatedContainer();

// Attributes:
public:

// Operations:
public:
	void add_gameobject(const ServerGameObjectPtr &gameobject);
	void remove_gameobject(const ServerGameObjectPtr &gameobject);

	void attach_gameobject(const ServerGameObjectPtr &gameobject);
	void detach_gameobject(const ServerGameObjectPtr &gameobject);
	
	void sync_dirty_gameobjects();
	void clear_dirty_gameobjects();

// Implementation:
private:
	void replicate_new_gameobject(const ServerGameObjectPtr &gameobject);
	void replicate_existing_gameobject(const ServerGameObjectPtr &gameobject);

	std::vector<ServerGameObjectPtr> objects;

	Player *player;
	ComponentSerializerPtr component_serializer;
};
typedef std::shared_ptr<ReplicatedContainer> ReplicatedContainerPtr;
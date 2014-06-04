#pragma once

#include "Components/replicated_properties.h"
#include <Totem/EventSystem.h>

typedef std::shared_ptr<Totem::IProperty> IPropertyPtr;
typedef std::shared_ptr<Totem::IComponent<>> IComponentPtr;
class GameObjectsRepository; typedef std::shared_ptr<GameObjectsRepository> GameObjectsRepositoryPtr;
class ServerGameObject; typedef std::shared_ptr<ServerGameObject> ServerGameObjectPtr; typedef std::weak_ptr<ServerGameObject> ServerGameObjectWeakPtr;
class Player; typedef std::shared_ptr<Player> PlayerPtr;

class ServerGameObject : public Totem::ComponentContainer<>, public Totem::PropertyContainer<>, public Totem::Ext::EventSystem<>, public ReplicatedProperties, public std::enable_shared_from_this<ServerGameObject>
{
// Enums:
public:
	enum StorageMode
	{
		STORAGE_TEMPORARY = 1,
		STORAGE_PERMANENT = 2
	};

// Construction:
private:
	// The constructors are private since gameobject creation has to be done through GameObjectManager
	ServerGameObject(const GameObjectsRepositoryPtr &gameobjects_repository, int id, const std::string &type);
	ServerGameObject(const GameObjectsRepositoryPtr &gameobjects_repository, int id, const std::string &type, const std::string &name);

public:
	~ServerGameObject();

// Attributes:
public:
	int get_id() const;
	ServerGameObjectPtr get_parent() const;
	const std::string &get_type() const;
	const std::string &get_name() const;

	bool is_destroyed() const;

    ServerGameObjectPtr find_child(int id);
    ServerGameObjectPtr find_child_by_type(const std::string &type);

	template<class ComponentType> std::shared_ptr<ComponentType> find_child_component()
	{
		for (auto it = children.begin(); it != children.end(); ++it)
		{
			auto object = *it;
			if(object->hasComponent<ComponentType>())
				return object->getComponent<ComponentType>();
		}
		return nullptr;
	}

	std::list<ServerGameObjectPtr> get_children();
    std::list<ServerGameObjectPtr> get_children_by_type(const std::string &type);
	template<class ComponentType> 
	std::list<std::shared_ptr<ComponentType>> get_children_components()
	{
		std::list<std::shared_ptr<ComponentType>> results;
		for (auto it = children.begin(); it != children.end(); ++it)
		{
			auto object = *it;
			if(object->hasComponent<ComponentType>())
				results.push_back(object->getComponent<ComponentType>());
		}
		return results;
	}

// Operations:
public:
	void tick(float time_elapsed);

	void destroy();

    void add_child(const ServerGameObjectPtr &gameobject, StorageMode mode);
	bool remove_child(const ServerGameObjectPtr &gameobject, StorageMode mode);

	void send_event(const clan::NetGameEvent &event, const PlayerPtr &player);

	void save_dirty_properties(const std::function<void()> &func);

// Signals:
public:
	clan::Signal<> sig_initialized;
	clan::Signal<const ServerGameObjectPtr &, bool /*moved*/> sig_child_added;
	clan::Signal<const ServerGameObjectPtr &, bool /*moved*/> sig_child_removed;

// Implementation:
private:
	void set_parent(const ServerGameObjectWeakPtr &parent);
    void clear_parent();

	void track_changes();

	void on_property_added(IPropertyPtr property);
	void on_property_removed(IPropertyPtr property);
	void on_component_added(IComponentPtr component);
	void on_component_removed(IComponentPtr component);

	int id;
    ServerGameObjectWeakPtr parent;
	std::string type;
	bool destroyed;
	
	Totem::Property<std::string> property_name;
	Totem::Property<bool> property_replicate;

	std::list<ServerGameObjectPtr> children;

	GameObjectsRepositoryPtr gameobjects_repository;

	clan::CallbackContainer slots;

	friend class GameObjectManager;
};


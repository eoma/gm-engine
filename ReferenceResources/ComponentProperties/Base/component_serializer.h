#pragma once

class ServerGameObject; typedef std::shared_ptr<ServerGameObject> ServerGameObjectPtr;

class ComponentSerializer
{
public:
	void create_and_add_component(const ServerGameObjectPtr &owner, const std::string &type, const std::string &name);

	clan::Signal<const ServerGameObjectPtr &/*owner*/, const std::string &/*type*/, const std::string &/*name*/> sig_create_component;
};

typedef std::shared_ptr<ComponentSerializer> ComponentSerializerPtr;

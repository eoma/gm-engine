#pragma once

#include "property_serializer.h"

class ComponentSerializer; typedef std::shared_ptr<ComponentSerializer> ComponentSerializerPtr;
class ServerGameObject; typedef std::shared_ptr<ServerGameObject> ServerGameObjectPtr;

class TemplateManager
{
public:
	TemplateManager(const ComponentSerializerPtr &component_serializer);

	void add_templates(const std::string template_filename);

	void apply(const std::string &template_name, const ServerGameObjectPtr &gameobject);

private:
	struct Property
	{
		PropertySerializer::PropertyType type_id;
		std::string name;
		std::string value;
	};

	struct Template
	{
		std::string name;
		std::vector<std::string> requires;
		std::vector<std::string> components;
		std::vector<Property> properties;
	};

	ComponentSerializerPtr component_serializer;

	std::vector<Template> templates;
};
typedef std::shared_ptr<TemplateManager> TemplateManagerPtr;
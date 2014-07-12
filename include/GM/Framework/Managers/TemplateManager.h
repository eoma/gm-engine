#pragma once

#include "../Utilities/PropertySerializer.h"

namespace GM {
	namespace Framework {
		class ComponentSerializer; typedef std::shared_ptr<ComponentSerializer> ComponentSerializerPtr;
		class Entity; typedef std::shared_ptr<Entity> EntityPtr;

		class TemplateManager {
		public:
			TemplateManager(const ComponentSerializerPtr &component_serializer);

			void add_templates(const std::string template_filename);

			void apply(const std::string &template_name, const EntityPtr &entity);

			struct Property {
				PropertySerializer::PropertyType type_id;
				std::string name;
				std::string value;
			};

			struct Template {
				std::string name;
				std::vector<std::string> requires;
				std::vector<std::string> components;
				std::vector<Property> properties;
			};

		private:
			void apply_components(const std::string &template_name, const EntityPtr &entity);
			void apply_properties(const std::string &template_name, const EntityPtr &entity);

			ComponentSerializerPtr component_serializer;

			std::vector<Template> templates;
		};
		typedef std::shared_ptr<TemplateManager> TemplateManagerPtr;
	}
}

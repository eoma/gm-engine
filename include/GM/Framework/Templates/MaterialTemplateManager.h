#pragma once

#include "../Utilities/PropertySerializer.h"

#include <string>
#include <memory>
#include <vector>
#include <functional>

namespace GM {
	namespace Framework {

		class Material; typedef std::shared_ptr<Material> MaterialPtr;

		class MaterialTemplateManager {
		public:
			MaterialTemplateManager();

			struct Property {
				PropertySerializer::PropertyType type_id;
				std::string name;
				std::string value;
			};

			struct Template {
				std::string name;
				std::vector<std::string> requires;
				std::string shader;
				std::vector<Property> properties;

				Template() : name(), requires(), shader(), properties() {}
			};

			void add_templates(const std::string &template_filename);

			void get(const std::string &template_name, const MaterialPtr &material, std::function<void(const MaterialTemplateManager::Template &)> func);

			

		private:
			void apply_requirement(const std::string &template_name, MaterialTemplateManager::Template &t);
			void apply_properties(const std::string &template_name, const MaterialPtr &material);
			std::vector<Template> templates;
		};
	}
}
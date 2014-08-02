#pragma once

#include <string>
#include <memory>
#include <vector>
#include <functional>

namespace GM {
	namespace Framework {

		class Mesh; typedef std::shared_ptr<Mesh> MeshPtr;

		class MeshTemplateManager {
		public:
			MeshTemplateManager();

			struct Template {
				std::string name;
				std::vector<std::string> requires;
				std::string filename;
				int mesh_index;

				Template() : name(), requires(), filename(), mesh_index(0) {}
			};

			void add_templates(const std::string &template_filename);

			void get(const std::string &template_name, std::function<void(const MeshTemplateManager::Template &)> func);

		private:
			void apply_requirement(const std::string &template_name, MeshTemplateManager::Template &t);
			std::vector<Template> templates;
		};
	}
}
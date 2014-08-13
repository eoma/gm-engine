#pragma once

#include <string>
#include <memory>
#include <vector>
#include <functional>

namespace GM {
	namespace Core { class Shader; typedef std::shared_ptr<Shader> ShaderPtr; }
	namespace Framework {

		class TextureTemplateManager {
		public:
			TextureTemplateManager();

			struct Template {
				std::string name;
				std::vector<std::string> requires;
				std::vector<std::string> images;
				std::string format;

				Template() : name(), requires(), images(), format() {}
			};

			void add_templates(const std::string &template_filename);

			void get(const std::string &template_name, std::function<void(const TextureTemplateManager::Template &)> func);

			

		private:
			void apply_requirement(const std::string &template_name, TextureTemplateManager::Template &t);
			std::vector<Template> templates;
		};
	}
}
#pragma once

#include <string>
#include <memory>
#include <vector>
#include <functional>

namespace GM {
	namespace Core { class Texture; typedef std::shared_ptr<Texture> TexturePtr; }
	namespace Framework {

		class TextureFormatTemplateManager {
		public:
			TextureFormatTemplateManager();

			struct Template {
				std::string name;
				std::vector<std::string> requires;
				std::string min_filter;
				std::string mag_filter;
				std::string wrap_s;
				std::string wrap_t;
				// TODO: Add all the things as far as texture parameters go...
				bool generate_mipmap;

				//TODO: default constructor here should set up the default "built into code" shader template (passthrough).
				Template() : name(), requires(), min_filter(), mag_filter(), wrap_s(), wrap_t(), generate_mipmap(false) {}
			};

			void add_templates(const std::string &template_filename);

			void get(const std::string &template_name, std::function<void(const TextureFormatTemplateManager::Template &)> func);

			

		private:
			void apply_requirement(const std::string &template_name, TextureFormatTemplateManager::Template &t);
			std::vector<Template> templates;
		};
	}
}
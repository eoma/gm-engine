#pragma once

#include <string>
#include <memory>
#include <vector>
#include <functional>

namespace GM {
	namespace Core { class Shader; typedef std::shared_ptr<Shader> ShaderPtr; }
	namespace Framework {

		class ShaderTemplateManager {
		public:
			ShaderTemplateManager();

			struct Template {
				std::string name;
				std::vector<std::string> requires;
				std::string vertex_shader;
				std::string fragment_shader;
				std::string geometry_shader;
				std::string tesselation_control_shader;
				std::string tesselation_evaluation_shader;
				std::string compute_shader;
				bool rasterizer_discard;

				//TODO: default constructor here should set up the default "built into code" shader template (passthrough).
				Template() : name(), requires(), 
					vertex_shader(), fragment_shader(), geometry_shader(), 
					tesselation_control_shader(), tesselation_evaluation_shader(), 
					compute_shader(), rasterizer_discard(false) {}
			};

			void add_templates(const std::string &template_filename);

			void get(const std::string &template_name, std::function<void(const ShaderTemplateManager::Template &)> func);

			

		private:
			void apply_requirement(const std::string &template_name, ShaderTemplateManager::Template &t);
			std::vector<Template> templates;
		};
	}
}
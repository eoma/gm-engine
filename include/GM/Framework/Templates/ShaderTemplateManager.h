#pragma once

#include <string>
#include <memory>
#include <vector>

namespace GM {
	namespace Core { class Shader; typedef std::shared_ptr<Shader> ShaderPtr; }
	namespace Framework {

		class ShaderTemplateManager {
		public:
			ShaderTemplateManager();

			void add_templates(const std::string &template_filename);

			void apply(const std::string &template_name, const Core::ShaderPtr &shader);

			struct Template {
				std::string name;
				std::vector<std::string> requires;
				std::string vertex_shader;
				std::string fragment_shader;
				std::string geometry_shader;
				std::string tesselation_control_shader;
				std::string tesselation_evaluation_shader;
				std::string compute_shader;

				//TODO: default constructor here should set up the default "built into code" shader template (passthrough).
			};

		private:
			void apply_template(const std::string &template_name, const Core::ShaderPtr &shader);
			std::vector<Template> templates;
		};
	}
}
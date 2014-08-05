#pragma once

#include <glm/glm.hpp>

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
				std::string type;
				std::vector<std::string> requires;
				std::string min_filter;			// nearest, linear, nearest_mipmap_nearest, linear_mipmap_nearest, nearest_mipmap_linear, linear_mipmap_linear. Defaults to nearest_mipmap_linear.
				std::string mag_filter;			// nearest or linear. Defaults to linear.
				std::string wrap_s;				// clamp_to_edge, clamp_to_border, mirrored_repeat, repeat, mirror_clamp_to_edge. Defaults to repeat.
				std::string wrap_t;				// clamp_to_edge, clamp_to_border, mirrored_repeat, repeat, mirror_clamp_to_edge. Defaults to repeat.
				std::string wrap_r;				// clamp_to_edge, clamp_to_border, mirrored_repeat, repeat, mirror_clamp_to_edge. Defaults to repeat.
				std::string depth_stencil_mode; // depth or stencil. Defaults to depth.
				int base_level;					// index of lowest defined mipmap level. Default is 0.
				std::string compare_func;		// lequal, gequal, less, greater, equal, notequal, always, never. Defaults to lequal.
				std::string compare_mode;		// compare or none. Defaults to none.
				float lod_bias;					// specifies a fixed bias value that is to be added to the level-of-detail parameter for the texture before texture sampling. Defaults to 0.
				float min_lod;					// limits the selection of highest resolution mipmap. Defaults to -1000.
				float max_lod;					// limits the selection of lowest resolution mipmap. Defaults to 1000.
				int max_level;					// sets the index of the highest defined mipmap level. Defaults to 1000.
				std::string swizzle_r;			// red, green, blue, alpha, zero, one. Defaults to red.
				std::string swizzle_g;			// red, green, blue, alpha, zero, one. Defaults to green.
				std::string swizzle_b;			// red, green, blue, alpha, zero, one. Defaults to blue.
				std::string swizzle_a;			// red, green, blue, alpha, zero, one. Defaults to alpha.
				std::string swizzle_rgba;		// red, green, blue, alpha, zero, one. Defaults to "red green blue alpha".
				glm::vec4 border_color;			// define the border values that should be used for border texels. Defaults to (0,0,0,0).
				bool generate_mipmap;

				//TODO: default constructor here should set up the default "built into code" shader template (passthrough).
				Template() : name("default"), type("2d"), requires(),
					min_filter("nearest_mipmap_linear"),
					mag_filter("linear"),
					wrap_s("repeat"),
					wrap_t("repeat"),
					wrap_r("repeat"),
					depth_stencil_mode("depth"),
					base_level(0),
					compare_func("lequal"),
					compare_mode("none"),
					lod_bias(0),
					min_lod(-1000),
					max_lod(1000),
					max_level(1000),
					swizzle_r("red"),
					swizzle_g("green"),
					swizzle_b("blue"),
					swizzle_a("alpha"),
					swizzle_rgba("red green blue alpha"),
					border_color(glm::vec4(0,0,0,0)),

					generate_mipmap(true) {}
			};

			void add_templates(const std::string &template_filename);

			void get(const std::string &template_name, std::function<void(const TextureFormatTemplateManager::Template &)> func);

			

		private:
			void apply_requirement(const std::string &template_name, TextureFormatTemplateManager::Template &t);
			std::vector<Template> templates;
		};
	}
}
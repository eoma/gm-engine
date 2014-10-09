#include <GM/Framework/Templates/TextureFormatTemplateManager.h>
#include <GM/Framework/Templates/TextureFormatTemplateParser.h>
#include <GM/Framework/IO/TextIO.h>

#include <GM/Core/GL/Shader.h>

#include <ClanLib/core.h>

using namespace GM;
using namespace Framework;
using namespace clan;

#if defined(_MSC_VER)
#	pragma warning (disable:4701) // Avoid warning about uninitialized forced_type
#endif

TextureFormatTemplateManager::TextureFormatTemplateManager() {

}

void TextureFormatTemplateManager::get(const std::string &template_name, std::function<void(const TextureFormatTemplateManager::Template &)> func) {
	for (auto it_template = templates.begin(); it_template != templates.end(); ++it_template)
	{
		if (StringHelp::compare(template_name, it_template->name, true) == 0)
		{
			for (auto it_require = it_template->requires.begin(); it_require != it_template->requires.end(); ++it_require)
			{
				apply_requirement((*it_require), (*it_template));
			}
			if(func) func((*it_template));

			return;
		}
	}
}

void TextureFormatTemplateManager::apply_requirement(const std::string &template_name, TextureFormatTemplateManager::Template &t) {
	for (auto it_template = templates.begin(); it_template != templates.end(); ++it_template)
	{
		if (StringHelp::compare(template_name, it_template->name, true) == 0)
		{
			if (t.type.empty() || t.type == "2d") t.type = it_template->type;
			if (t.min_filter.empty() || t.min_filter == "nearest_mipmap_linear") t.min_filter = it_template->min_filter;
			if (t.mag_filter.empty() || t.mag_filter == "linear") t.mag_filter = it_template->mag_filter;
			if (t.wrap_s.empty() || t.wrap_s == "repeat") t.wrap_s = it_template->wrap_s;
			if (t.wrap_t.empty() || t.wrap_t == "repeat") t.wrap_t = it_template->wrap_t;
			if (t.wrap_r.empty() || t.wrap_r == "repeat") t.wrap_r = it_template->wrap_r;
			if (t.depth_stencil_mode.empty() || t.depth_stencil_mode == "depth") t.depth_stencil_mode = it_template->depth_stencil_mode;
			if (t.base_level == 0) t.base_level = it_template->base_level;
			if (t.compare_func.empty() || t.compare_func == "lequal") t.compare_func = it_template->compare_func;
			if (t.compare_mode.empty() || t.compare_mode == "none") t.compare_mode = it_template->compare_mode;
			if (t.lod_bias == 0) t.lod_bias = it_template->lod_bias;
			if (t.min_lod == -1000) t.min_lod = it_template->min_lod;
			if (t.max_lod == 1000) t.max_lod = it_template->max_lod;
			if (t.max_level == 1000) t.max_level = it_template->max_level;
			if (t.swizzle_r.empty() || t.swizzle_r == "red") t.swizzle_r = it_template->swizzle_r;
			if (t.swizzle_g.empty() || t.swizzle_g == "green") t.swizzle_g = it_template->swizzle_g;
			if (t.swizzle_b.empty() || t.swizzle_b == "blue") t.swizzle_b = it_template->swizzle_b;
			if (t.swizzle_a.empty() || t.swizzle_a == "alpha") t.swizzle_a = it_template->swizzle_a;
			if (t.swizzle_rgba.empty() || t.swizzle_rgba == "red green blue alpha") t.swizzle_rgba = it_template->swizzle_rgba;
			if (t.border_color == glm::vec4(0, 0, 0, 0)) t.border_color = it_template->border_color;
			if (t.generate_mipmap) t.generate_mipmap = it_template->generate_mipmap;
			if (t.gl_texture_format.empty()) t.gl_texture_format = it_template->gl_texture_format;
			return;
		}
	}
}

void TextureFormatTemplateManager::add_templates(const std::string &template_filename) {
	try
	{
		TextureFormatTemplateParser::parse_templates(
			TextIO::load_contents(template_filename),
			[this](const TextureFormatTemplateManager::Template &t)
		{
			templates.push_back(t);
		});
	}
	catch (Exception e)
	{
		throw Exception(string_format("%1: %2", template_filename, e.what()));
	}
}

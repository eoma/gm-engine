#include "GM/Core/Utilities/Tools.h"

#include "GM/Core/GL/Shader.h"

namespace GM {
namespace Core {

void print_shader_info(const ShaderPtr &shader, std::ostream &out)
{
	for (const auto &info : shader->get_attribute_infos())
	{
		out << "Attribute: " << info.name << std::endl;
		out << "Type: " << std::hex << std::showbase << info.type << std::dec << std::endl;
		out << "Size: " << info.size << std::endl;
		out << "Location: " << info.location << std::endl;

		out << std::endl;
	}

	out << "-----" << std::endl << std::endl;

	for (const auto &info : shader->get_uniform_infos())
	{
		out << "Uniform: " << info.name << std::endl;
		out << "Type: " << std::hex << std::showbase << info.type << std::dec << std::endl;
		out << "Size: " << info.size << std::endl;
		out << "Location: " << info.location << std::endl;

		out << std::endl;
	}

	out << "-----" << std::endl << std::endl;

	for (const auto &info : shader->get_uniform_block_infos())
	{
		out << "Uniform Block: " << info.name << std::endl;
		out << "Size: " << info.size << std::endl;
		out << "Index: " << info.index << std::endl;
		out << "Active uniforms[" << info.active_uniforms.size() << "] = {" << std::endl;
		out << std::endl;

		for (const auto &active_uniform : info.active_uniforms)
		{
			out << "\tUniform: " << active_uniform.name << std::endl;
			out << "\tType: " << std::hex << std::showbase << active_uniform.type << std::dec << std::endl;
			out << "\tSize: " << active_uniform.size << std::endl;
			out << "\tOffset: " << active_uniform.offset << std::endl;
			out << "\tArray stride: " << active_uniform.array_stride << std::endl;
			out << "\tMatrix stride: " << active_uniform.matrix_stride << std::endl;

			out << std::endl;
		}


		out << "}" << std::endl;

		out << std::endl;
	}
}

} // namespace Core
} // namespace GM

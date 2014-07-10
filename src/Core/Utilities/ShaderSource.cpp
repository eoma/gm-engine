#include "GM/Core/Utilities/ShaderSource.h"

namespace GM {
namespace Core {

ShaderSource::ShaderSource(const std::string &name,
	const std::string &content,
	const unsigned int shader_type)
: name(name)
, content(content)
, shader_type(shader_type)
{
}

} // namespace Core
} // namespace GM


#include <GM/Framework/IO/ShaderIO.h>
#include <ClanLib/core.h>

using namespace GM;
using namespace Framework;
using namespace clan;

#if defined(_MSC_VER)
#	pragma warning (disable:4701) // Avoid warning about uninitialized forced_type
#endif

ShaderIO::ShaderIO() 
{
}

std::string ShaderIO::load_contents(const std::string &file_name)
{
	if (!clan::FileHelp::file_exists(file_name)) 
	{
		throw std::runtime_error("Shader file does not exist!");
	}
		
	return clan::File::read_text(file_name);
}
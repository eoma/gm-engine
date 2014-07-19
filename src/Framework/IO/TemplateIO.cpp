
#include <GM/Framework/IO/TemplateIO.h>
#include <GM/Framework/Utilities/PropertySerializer.h>

#include <ClanLib/core.h>

using namespace GM;
using namespace Framework;
using namespace clan;

#if defined(_MSC_VER)
#	pragma warning (disable:4701) // Avoid warning about uninitialized forced_type
#endif

TemplateIO::TemplateIO() 
{
}

std::string TemplateIO::load_contents(const std::string &template_filename)
{
	if (!clan::FileHelp::file_exists(template_filename))
	{
		throw std::runtime_error("Template file does not exist!");
	}

	return File::read_text(template_filename);
}

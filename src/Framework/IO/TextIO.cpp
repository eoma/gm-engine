
#include <GM/Framework/IO/TextIO.h>
#include <GM/Framework/Utilities/PropertySerializer.h>

#include <ClanLib/core.h>

using namespace GM;
using namespace Framework;
using namespace clan;

#if defined(_MSC_VER)
#	pragma warning (disable:4701) // Avoid warning about uninitialized forced_type
#endif

TextIO::TextIO()
{
}

std::string TextIO::load_contents(const std::string &filename)
{
	if (!clan::FileHelp::file_exists(filename))
	{
		throw clan::Exception(string_format("Text file %1 does not exist!", filename));
	}

	return File::read_text(filename);
}

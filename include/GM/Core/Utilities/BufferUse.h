#pragma once

namespace GM {
namespace Core {

struct BufferUse
{
	unsigned int name;
	unsigned int type;

	BufferUse() : name(0), type(0) {};

	static int compare(const BufferUse &current, const BufferUse &other);
};

bool operator< (const BufferUse &current, const BufferUse &other);
bool operator== (const BufferUse &current, const BufferUse &other);

} // namespace Core
} // namespace GM

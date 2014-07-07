#pragma once

namespace GM {
namespace Framework {

struct BufferUse
{
	unsigned int name;
	unsigned int type;

	BufferUse() : name(0), type(0) {};

	static int compare(const BufferUse &current, const BufferUse &other);
};

bool operator< (const BufferUse &current, const BufferUse &other);
bool operator== (const BufferUse &current, const BufferUse &other);

} // namespace Framework
} // namespace GM

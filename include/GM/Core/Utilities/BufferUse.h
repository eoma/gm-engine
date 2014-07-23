#pragma once

#include <memory>

namespace GM {
namespace Core {

class BufferObject; typedef std::shared_ptr<BufferObject> BufferObjectPtr;

struct BufferUse
{
	BufferObjectPtr buffer;
	unsigned int type;

	BufferUse() : buffer(nullptr), type(0) {};

	static int compare(const BufferUse &current, const BufferUse &other);
};

bool operator< (const BufferUse &current, const BufferUse &other);
bool operator== (const BufferUse &current, const BufferUse &other);

} // namespace Core
} // namespace GM

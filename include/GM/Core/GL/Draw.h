#pragma once

#include "../Utilities/DrawCommand.h"

#include <GL/gl3w.h>

namespace GM {
namespace Core {

class Draw
{
public:
	static void draw(const DrawCommand &command);

	// For DrawArrays
	//static void draw_indirect(const GLenum mode, void* indirect_buffer_offset);

	// For DrawElements
	//static void draw_indirect(const GLenum mode, const GLenum index_type, void* indirect_buffer_offset);

	// May be used for glMultiDrawIndirect
	// The *DrawCommandCollection should contain the draw mode, and if 
	// ElementDrawCommandCollection, the index type (byte, short or int)
	//
	// The *DrawCommandCollection must contain a pointer into an indirect
	// draw command buffer and how many commands are to be processed
	//static void draw(const ElementDrawCommanCollection &command_collection);
	//static void draw(const ArrayDrawCommanCollection &command_collection);

};

} // namespace Core
} // namespace GM

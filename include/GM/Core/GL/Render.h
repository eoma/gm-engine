#pragma once

#include "../Utilities/RenderCommand.h"

#include <GL/gl3w.h>

namespace GM {
namespace Core {

class Render
{
public:
	static void render(const RenderCommand &command);

	// For rendering without IBO
	//static void render(const GLenum mode, void* indirect_buffer_offset);

	// For rendering with IBO
	//static void render(const GLenum mode, const GLenum index_type, void* indirect_buffer_offset);

	// May be used for glMultiDraw{Arrays,Elements}Indirect
	// The *RenderCommandCollection should contain the render mode, and if 
	// ElementRenderCommandCollection, the index type (byte, short or int)
	//
	// The *RenderCommandCollection must contain a pointer into an indirect
	// render command buffer and how many commands are to be processed
	//static void render(const ElementRenderCommanCollection &command_collection);
	//static void render(const ArrayRenderCommanCollection &command_collection);

};

} // namespace Core
} // namespace GM

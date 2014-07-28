#pragma once

#include <GL/gl3w.h>

namespace GM {
namespace Core {

struct RenderElementsIndirectCommand
{
	unsigned int count; // How many elements should rendered, or how many indices should we read
	unsigned int instance_count; // how many instances of the should be drawn?
	unsigned int first_index; // where in the buffer bound as IBO (GL_ELEMENT_ARRAY_BUFFER) do we find the indices? Dependent on index type (eg. byte or int)

	// Only relevant if gl version >= 4.0
	unsigned int base_vertex; // at what vertex index should we pull data from? this index is added to the index pulled from the IBO.
	unsigned int base_instance; // same functionality as base vertex. Set to 0 (zero) if gl version < 4.2.
};

struct RenderArraysIndirectCommand
{
	unsigned int count; // How many elements should rendered, or how many indices should we read
	unsigned int instance_count; // how many instances of should be drawn?
	unsigned int first; // The first vertex index to start drawing from
	unsigned int base_instance; // Set to 0 if gl version < 4.2
};

class RenderCommand
{
public:
	RenderCommand(bool is_indexed, unsigned int count, unsigned int instance_count, unsigned int first, unsigned int base_vertex = 0, unsigned int base_instance = 0);

	operator RenderElementsIndirectCommand() const;
	operator RenderArraysIndirectCommand() const;

	// whether or not this is a RenderArrays or RenderElements command
	bool is_indexed; 

	// The draw mode, eg GL_POINTS, GL_LINE_STRIP, etc.
	GLenum mode;

	// if indexed, either GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT or GL_UNSIGNED_INTEGER
	GLenum index_type; 

	unsigned int count;
	unsigned int instance_count;
	unsigned int first;
	unsigned int base_vertex;
	unsigned int base_instance;

};

} // namespace Core
} // namespace GM

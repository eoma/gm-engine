#include "GM/Core/GL/Render.h"

namespace GM {
namespace Core {

void Render::render(const RenderCommand &command)
{
	if (command.is_indexed)
	{
		unsigned int index_type_size = 0;
		if (command.index_type == GL_UNSIGNED_BYTE)
		{
			index_type_size = 1;
		}
		else if (command.index_type == GL_UNSIGNED_SHORT)
		{
			index_type_size = 2;
		}
		else if (command.index_type == GL_UNSIGNED_INT)
		{
			index_type_size = 4;
		}

		if (command.is_instanced)
		{
			if (command.base_instance > 0) { 
				glDrawElementsInstancedBaseVertexBaseInstance(command.mode,
					                                      command.count,
					                                      command.index_type,
					                                      reinterpret_cast<void*>(command.first * index_type_size),
					                                      command.instance_count,
					                                      command.base_vertex,
					                                      command.base_instance);
			}
			else
			{
				// FIXME: Remove this when OS X supports OpenGL 4.2
				glDrawElementsInstancedBaseVertex(command.mode,
					                          command.count,
					                          command.index_type,
					                          reinterpret_cast<void*>(command.first * index_type_size),
					                          command.instance_count,
					                          command.base_vertex);
			}
		}
		else
		{
			glDrawElementsBaseVertex(command.mode,
			                         command.count,
			                         command.index_type,
			                         reinterpret_cast<void*>(command.first * index_type_size),
			                         command.base_vertex);
		}
	}
	else
	{
		if (command.is_instanced)
		{
			if (command.base_instance > 0)
			{
				// Render using RenderArrays*
				glDrawArraysInstancedBaseInstance(command.mode,
					                          command.first,
					                          command.count,
					                          command.instance_count,
					                          command.base_instance);
			}
			else
			{
				glDrawArraysInstanced(command.mode,
					              command.first,
					              command.count,
					              command.instance_count);
			}
		}
		else
		{
			glDrawArrays(command.mode,
			             command.first,
			             command.count);
		}
	}
}

} // namespace Core
} // namespace GM

#pragma once

#include "../../Core/Utilities/VaoLayout.h"

#include <map>


namespace GM {

namespace Core {
	class VertexArrayObject; typedef std::shared_ptr<VertexArrayObject> VertexArrayObjectPtr;
}

namespace Framework {

class VaoManager
{
public:
	VaoManager();
	~VaoManager();

	Core::VertexArrayObjectPtr get_vao_for(const Core::VaoLayout &layout);

	/**
	 * Will locate a VAO Layout for a constructed VertexArrayObject that this manager
	 * know about
	 * 
	 * @return a defined VAO Layout or an empty vao layout
	 */
	Core::VaoLayout get_vao_layout_for(const Core::VertexArrayObjectPtr &vao) const;

private:
	Core::VertexArrayObjectPtr build_vao(const Core::VaoLayout &layout);

private:
	// These could be replaced by a bimap
	std::map<Core::VaoLayout, Core::VertexArrayObjectPtr> layouts_to_vaos;
	std::map<Core::VertexArrayObjectPtr, Core::VaoLayout> vaos_to_layouts;

};

typedef std::shared_ptr<VaoManager> VaoManagerPtr;

} // namespace Framework
} // namespace GM

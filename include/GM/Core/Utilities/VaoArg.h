#pragma once

#include "VertexAttribute.h"

#include <glm/glm.hpp>

namespace GM {
namespace Core {

template <class Data>
class VaoArg
{
public:
	VaoArg(unsigned int index);

	operator VertexAttribute() const {
		return attrib;
	}
private:
	VertexAttribute attrib;
};

// Specializations

// glm vector
template<> VaoArg<glm::vec2>::VaoArg(unsigned int index);
template<> VaoArg<glm::vec3>::VaoArg(unsigned int index);
template<> VaoArg<glm::vec4>::VaoArg(unsigned int index);

// glm matrix
template<> VaoArg<glm::mat2>::VaoArg(unsigned int index);
template<> VaoArg<glm::mat3>::VaoArg(unsigned int index);
template<> VaoArg<glm::mat4>::VaoArg(unsigned int index);
template<> VaoArg<glm::mat4x3>::VaoArg(unsigned int index);
template<> VaoArg<glm::mat4x2>::VaoArg(unsigned int index);

// Primitives
template<> VaoArg<float>::VaoArg(unsigned int index);
template<> VaoArg<int>::VaoArg(unsigned int index);
template<> VaoArg<unsigned int>::VaoArg(unsigned int index);

} // namespace Core
} // namespace GM

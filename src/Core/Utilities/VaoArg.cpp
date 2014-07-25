#include "GM/Core/Utilities/VaoArg.h"

#include <GL/gl3w.h>

namespace GM {
namespace Core {

//
// GLM vector
//

template<> VaoArg<glm::vec2>::VaoArg(unsigned int index)
: attrib(index, 2, GL_FLOAT, sizeof(float), false, 1)
{}

template<> VaoArg<glm::vec3>::VaoArg(unsigned int index)
: attrib(index, 3, GL_FLOAT, sizeof(float), false, 1)
{}

template<> VaoArg<glm::vec4>::VaoArg(unsigned int index)
: attrib(index, 4, GL_FLOAT, sizeof(float), false, 1)
{}


//
// GLM matric
//

template<> VaoArg<glm::mat2>::VaoArg(unsigned int index)
: attrib(index, 2, GL_FLOAT, sizeof(float), false, 2)
{}

template<> VaoArg<glm::mat3>::VaoArg(unsigned int index)
: attrib(index, 3, GL_FLOAT, sizeof(float), false, 3)
{}

template<> VaoArg<glm::mat4>::VaoArg(unsigned int index)
: attrib(index, 4, GL_FLOAT, sizeof(float), false, 4)
{}

template<> VaoArg<glm::mat4x3>::VaoArg(unsigned int index)
: attrib(index, 4, GL_FLOAT, sizeof(float), false, 3)
{}

template<> VaoArg<glm::mat4x2>::VaoArg(unsigned int index)
: attrib(index, 4, GL_FLOAT, sizeof(float), false, 2)
{}


//
// Primitives
//

template<> VaoArg<float>::VaoArg(unsigned int index)
: attrib(index, 1, GL_FLOAT, sizeof(float), false, 1)
{}

template<> VaoArg<int>::VaoArg(unsigned int index)
: attrib(index, 1, GL_INT, sizeof(unsigned int), false, 1)
{}

template<> VaoArg<unsigned int>::VaoArg(unsigned int index)
: attrib(index, 1, GL_UNSIGNED_INT, sizeof(unsigned int), false, 1)
{}

} // namespace Core
} // namespace GM

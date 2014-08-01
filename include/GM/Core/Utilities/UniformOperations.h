#pragma once

#include "../GL/Texture.h"

#include <glm/glm.hpp>

#include <memory>

namespace GM {
namespace Core {

// Primitives
void update_uniform(unsigned int program, int location, char data);
void update_uniform(unsigned int program, int location, unsigned char data);

void update_uniform(unsigned int program, int location, short data);
void update_uniform(unsigned int program, int location, unsigned short data);

void update_uniform(unsigned int program, int location, int data);
void update_uniform(unsigned int program, int location, unsigned int data);

void update_uniform(unsigned int program, int location, float data);

// Vectors
void update_uniform(unsigned int program, int location, const glm::vec2& data);
void update_uniform(unsigned int program, int location, const glm::ivec2& data);
void update_uniform(unsigned int program, int location, const glm::uvec2& data);

void update_uniform(unsigned int program, int location, const glm::vec3& data);
void update_uniform(unsigned int program, int location, const glm::ivec3& data);
void update_uniform(unsigned int program, int location, const glm::uvec3& data);

void update_uniform(unsigned int program, int location, const glm::vec4& data);
void update_uniform(unsigned int program, int location, const glm::ivec4& data);
void update_uniform(unsigned int program, int location, const glm::uvec4& data);

// Matrices
void update_uniform(unsigned int program, int location, const glm::mat2& data);
void update_uniform(unsigned int program, int location, const glm::mat3& data);
void update_uniform(unsigned int program, int location, const glm::mat4& data);

// Textures
void update_uniform(unsigned int program, int location, const TexturePtr &data);

} // namespace Core
} // namespace GM

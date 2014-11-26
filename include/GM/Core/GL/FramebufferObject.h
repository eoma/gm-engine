#pragma once

#include <GL/gl3w.h>

#include <glm/glm.hpp>

#include <memory>
#include <map>

namespace GM {
namespace Core {

class RenderbufferObject; typedef std::shared_ptr<RenderbufferObject> RenderbufferObjectPtr;
class Shader; typedef std::shared_ptr<Shader> ShaderPtr;
class Texture; typedef std::shared_ptr<Texture> TexturePtr;

class FramebufferObject
{
public:
	FramebufferObject();
	~FramebufferObject();

	unsigned int get_handle() const { return handle; }

	// Bind this framebuffer
	void bind();
	void unbind();

	// Checks if this is a valid framebuffer
	void check();

	void add(unsigned int attachment, const RenderbufferObjectPtr &render_buffer);
	void add(unsigned int attachment, const TexturePtr &render_texture);

	bool has_attachment(unsigned int attachment) const;

	bool has_render_texture(unsigned int attachment) const;
	bool has_render_buffer(unsigned int attachment) const;

	TexturePtr get_render_texture(unsigned int attachment) const { return render_textures.at(attachment); }
	RenderbufferObjectPtr get_render_buffer(unsigned int attachment) const { return render_buffers.at(attachment); }

	glm::vec4 pick(int x, int y, unsigned int attachment);

private:
	unsigned int handle;

	std::map<unsigned int, TexturePtr> render_textures;
	std::map<unsigned int, RenderbufferObjectPtr> render_buffers;
};

} // namespace Core
} // namespace GM

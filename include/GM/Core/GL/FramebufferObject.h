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

	// Will fetch texture handles from render textures and buffers and recheck.
	// Useful if texture handles has changed in the shared pointers
	void rebuild();

	/**
	 * Get and set draw buffer assignment.
	 * The key in the map refers to draw buffer index (unsigned int, the value refers to the actual buffer or texture attachment.
	 * May be easier to manage than standard arrays/vectors
	 *
	 * Draw buffer index is specified in the fragment shader.
	 * Example: layout(location = some_index) out vec4 colour;
	 */
	void set_draw_buffer_as_map(const std::map<unsigned int, GLenum> &draw_buffer_assignment);
	const std::map<unsigned int, GLenum> &get_draw_buffer_as_map() const;

	// Checks if this is a valid framebuffer
	void check();

	void add(GLenum attachment, const RenderbufferObjectPtr &render_buffer);
	void add(GLenum attachment, const TexturePtr &render_texture);

	bool has_attachment(GLenum attachment) const;

	bool has_render_texture(GLenum attachment) const;
	bool has_render_buffer(GLenum attachment) const;

	TexturePtr get_render_texture(GLenum attachment) const { return render_textures.at(attachment); }
	RenderbufferObjectPtr get_render_buffer(GLenum attachment) const { return render_buffers.at(attachment); }

	glm::vec4 pick(int x, int y, GLenum attachment);

private:
	void set_render_texture(GLenum attachment, const TexturePtr &render_texture);
	void set_render_buffer(GLenum attachment, const RenderbufferObjectPtr &render_buffer);

private:
	unsigned int handle;

	std::map<GLenum, TexturePtr> render_textures;
	std::map<GLenum, RenderbufferObjectPtr> render_buffers;
	std::map<unsigned int, GLenum> draw_buffer_assignment;
};

} // namespace Core
} // namespace GM

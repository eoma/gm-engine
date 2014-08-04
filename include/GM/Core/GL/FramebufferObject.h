#pragma once

#include <GL/gl3w.h>

#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace GM {
namespace Core {

class RenderbufferObject; typedef std::shared_ptr<RenderbufferObject> RenderbufferObjectPtr;
class Shader; typedef std::shared_ptr<Shader> ShaderPtr;
class Texture; typedef std::shared_ptr<Texture> TexturePtr;

class FramebufferObject
{
public:
	FramebufferObject(unsigned int w, unsigned int h);
	~FramebufferObject();

	void bind();
	void unbind();

	void bind_rt(const ShaderPtr &active_program, unsigned int index_offset);
	void unbind_rt(unsigned int index_offset);

	void add(unsigned int attachment, const RenderbufferObjectPtr &render_buffer);
	void add(unsigned int attachment, unsigned int texture_type, const std::string &sampler_name, const TexturePtr &render_texture);

	void check();
	glm::vec4 pick(int x, int y, unsigned int attachment);

	TexturePtr get_render_texture(unsigned int index) const { return render_textures[index]; }

	unsigned int get_handle() const { return handle; }
private:
	unsigned int find_attachment_index(unsigned int attachment);
	unsigned int handle;

	unsigned int w;
	unsigned int h;

	std::vector<RenderbufferObjectPtr> render_buffers;
	std::vector<TexturePtr> render_textures;
	std::vector<std::string> render_samplers;
	std::vector<unsigned int> attachments;
};

} // namespace Core
} // namespace GM

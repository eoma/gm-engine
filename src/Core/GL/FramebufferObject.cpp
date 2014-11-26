#include "GM/Core/GL/FramebufferObject.h"

#include "GM/Core/GL/RenderbufferObject.h"
#include "GM/Core/GL/Shader.h"
#include "GM/Core/GL/Texture.h"

#include "GM/Core/Utilities/UniformOperations.h"

namespace GM {
namespace Core {

FramebufferObject::FramebufferObject()
{
	glGenFramebuffers(1, &handle);
	bind();
}

FramebufferObject::~FramebufferObject()
{
	glDeleteFramebuffers(1, &handle);
}

void FramebufferObject::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

void FramebufferObject::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FramebufferObject::add(unsigned int attachment, const RenderbufferObjectPtr &render_buffer)
{
	if (has_render_texture(attachment)) {
		render_textures.erase(attachment);
	}
	render_buffers[attachment] = render_buffer;

	render_buffer->bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, render_buffer->get_handle());
}

void FramebufferObject::add(unsigned int attachment, const TexturePtr &render_texture)
{
	if (has_render_buffer(attachment)) {
		render_buffers.erase(attachment);
	}
	render_textures[attachment] = render_texture;

	render_texture->bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, render_texture->get_type(), render_texture->get_handle(), 0);
}

void FramebufferObject::check()
{
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if( status != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("Can't initialize an FramebufferObject render texture. FramebufferObject initialization failed.");
}

bool FramebufferObject::has_attachment(unsigned int attachment) const
{
	return has_render_buffer(attachment) || has_render_texture(attachment);
}

bool FramebufferObject::has_render_buffer(unsigned int attachment) const
{
	return render_buffers.find(attachment) != render_buffers.end();
}

bool FramebufferObject::has_render_texture(unsigned int attachment) const
{
	return render_textures.find(attachment) != render_textures.end();
}

glm::vec4 FramebufferObject::pick(int x, int y, unsigned int attachment)
{
	//TODO: Should rather get the value from GL_MAX_COLOR_ATTACHMENTS for max attachment id
	if(attachment < GL_COLOR_ATTACHMENT0 || attachment > GL_COLOR_ATTACHMENT15)
		throw std::runtime_error("Attachment to pick an FramebufferObject must be a color attachment.");

	auto height = render_buffers[attachment]->get_height();

	//Bind FramebufferObject
	bind();

	//Define which COLOR ATTACHMENT TO READ FROM
	glReadBuffer(attachment);

	//Read the pixel data at the location x,y
	unsigned char data[4] = {0};
	glReadPixels (x, height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);

	//Reset the read buffer to the back buffer
	//glReadBuffer(GL_BACK);

	//Unbind FramebufferObject
	unbind();

	//Return color data
	return glm::vec4(data[0], data[1], data[2], data[3]);
}

} // namespace Core
} // namespace GM

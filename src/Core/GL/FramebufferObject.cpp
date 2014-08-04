#include "GM/Core/GL/FramebufferObject.h"

#include "GM/Core/GL/RenderbufferObject.h"
#include "GM/Core/GL/Texture.h"

namespace GM {
namespace Core {

FramebufferObject::FramebufferObject(unsigned int w, unsigned int h)
	: w(w), h(h)
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

//void FramebufferObject::bind_rt(unsigned int active_program, unsigned int index_offset)
//{
//	for(unsigned int i = 0; i < render_textures.size(); ++i)
//	{
//		glActiveTexture(GL_TEXTURE0 + i+index_offset);
//		render_textures[i]->bind();
//		//render_samplers[i]->bind(i+(int)index_offset, active_program);
//	}
//}

//void FramebufferObject::unbind_rt(unsigned int index_offset)
//{
//	for(unsigned int i = 0; i < render_textures.size(); ++i)
//	{
//		glActiveTexture(GL_TEXTURE0 + i+index_offset);
//		render_textures[i]->unbind();
//	}
//}

void FramebufferObject::add(unsigned int attachment, const RenderbufferObjectPtr &render_buffer)
{
	attachments.push_back(attachment);
	render_buffers.push_back(render_buffer);

	render_buffer->bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, render_buffer->get_handle());
}

//void FramebufferObject::add(unsigned int attachment, unsigned int texture_type, const std::string &sampler_name, const TexturePtr &render_texture)
//{
//	render_textures.push_back(render_texture);
//	//render_samplers.push_back(std::make_shared<Uniform>(sampler_name));
//
//	render_texture->bind();
//	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texture_type, render_texture->get_handle(), 0);
//}

void FramebufferObject::check()
{
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if( status != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("Can't initialize an FramebufferObject render texture. FramebufferObject initialization failed.");
}

unsigned int FramebufferObject::find_attachment_index(unsigned int attachment)
{
	for(unsigned int i = 0; i < attachments.size(); i++)
	{
		if(attachments[i] == attachment)
			return i;
	}

	throw std::runtime_error("The attachment doesn't exist in this FramebufferObject.");
}

//glm::vec4 FramebufferObject::pick(int x, int y, unsigned int attachment)
//{
//	//TODO: Should rather get the value from GL_MAX_COLOR_ATTACHMENTS for max attachment id
//	if(attachment < GL_COLOR_ATTACHMENT0 || attachment > GL_COLOR_ATTACHMENT15)
//		throw std::runtime_error("Attachment to pick an FramebufferObject must be a color attachment.");
//
//	auto height = render_targets[ find_attachment_index(attachment) ]->getHeight();
//
//	//Bind FramebufferObject
//	bind();
//
//	//Define which COLOR ATTACHMENT TO READ FROM
//	glReadBuffer(attachment);
//
//	//Read the pixel data at the location x,y
//	unsigned char data[4] = {0};
//	glReadPixels (x, height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
//
//	//Reset the read buffer to the back buffer
//	//glReadBuffer(GL_BACK);
//
//	//Unbind FramebufferObject
//	unbind();
//
//	//Return color data
//	return glm::vec4(data[0], data[1], data[2], data[3]);
//}

} // namespace Core
} // namespace GM

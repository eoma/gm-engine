#pragma once

#include "GM/Framework/Totem/Totem.h"
#include "GM/Framework/Components/IRenderPassComponent.h"
#include "GM/Core/GL/FramebufferObject.h"

namespace GM {
	namespace Core {
		class Shader; typedef std::shared_ptr<Shader> ShaderPtr;
		class VertexArrayObject; typedef std::shared_ptr<VertexArrayObject> VertexArrayObjectPtr;
	} // Core
}


class SobelFilter : public GM::Framework::Component<SobelFilter>, public GM::Framework::IRenderPassComponent
{
public:
	SobelFilter(const GM::Framework::EntityPtr &owner, const std::string &name = std::string());
	virtual ~SobelFilter();

	std::string get_type() const override { return get_static_type(); }

	void build() override;

	void pass(GM::Framework::RenderSystem &render_system) override;

	bool uses_render_texture_from_camera() const override { return true; }
	void set_input_texture(const GM::Core::TexturePtr &input_texture) override;
	void set_output_texture(const GM::Core::TexturePtr &input_texture) override;

public:
	static std::string get_static_type() { return std::string("SobelFilter"); }

private:
	void construct_shader();
	void build_fbos();

private:
	bool satisfied_and_ready;

	GM::Core::FramebufferObject fbo_first_pass;

	GM::Core::TexturePtr input_texture;
	GM::Core::TexturePtr output_texture;

	GM::Core::ShaderPtr shader;
	GM::Core::VertexArrayObjectPtr vao;

	int input_texture_uniform;
};


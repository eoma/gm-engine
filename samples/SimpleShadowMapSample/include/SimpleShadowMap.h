#pragma once

#include "GM/Framework/Totem/Totem.h"
#include "GM/Framework/Components/IRenderPassComponent.h"

#include <memory>

namespace GM {

	namespace Core {
		class FramebufferObject; typedef std::shared_ptr<FramebufferObject> FramebufferObjectPtr;
	}

	namespace Framework {
		class TextureManager; typedef std::shared_ptr<TextureManager> TextureManagerPtr;

		class Camera; typedef std::shared_ptr<Camera> CameraPtr;
	}
}

class SimpleShadowMap
: public GM::Framework::IRenderPassComponent
, public GM::Framework::Component<SimpleShadowMap>
{
public:
	SimpleShadowMap(const GM::Framework::EntityPtr &owner, const GM::Framework::TextureManagerPtr &texture_manager, const std::string &name = std::string());
	virtual ~SimpleShadowMap();

	std::string get_type() const override { return get_static_type(); }
	static std::string get_static_type() { return "SimpleShadowMap"; }

	void build() override;
	void pass(GM::Framework::RenderSystem &render_system) override;

	bool uses_render_texture_from_camera() const override { return false; }
	void set_input_texture(const GM::Core::TexturePtr &) override {}
	void set_output_texture(const GM::Core::TexturePtr &) override {};

private:
	GM::Framework::TextureManagerPtr texture_manager;

	GM::Core::FramebufferObjectPtr framebuffer;
	GM::Core::TexturePtr shadow_map;

	GM::Framework::CameraPtr camera;

	GM::Framework::Property<glm::vec3> position;
	GM::Framework::Property<glm::quat> orientation;
};

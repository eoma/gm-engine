#include "SimpleShadowMap.h"

#include "GM/Framework/Entity.h"
#include "GM/Framework/DefinitionsPropertyNames.h"
#include "GM/Framework/Components/Camera.h"
#include "GM/Framework/Managers/TextureManager.h"
#include "GM/Framework/Systems/RenderSystem.h"

#include "GM/Core/GL/FramebufferObject.h"
#include "GM/Core/Utilities/TextureFactory.h"

#include "ClanLib/core.h"

using namespace GM;

SimpleShadowMap::SimpleShadowMap(const Framework::EntityPtr &owner, const Framework::TextureManagerPtr &texture_manager, const std::string &name)
: Framework::Component<SimpleShadowMap>(owner, name)
, texture_manager(texture_manager)
{
	position = owner->add<glm::vec3>(GM_PROPERTY_POSITION, glm::vec3());
	orientation = owner->add<glm::quat>(GM_PROPERTY_ORIENTATION, glm::quat());

	std::string shadow_map_name = clan::string_format("%1.simple_shadow_map", owner->get_name());

	Core::TextureFactory::TextureData texture_data;
	texture_data.texture_format = GL_TEXTURE_2D;
	texture_data.internal_format = GL_DEPTH_COMPONENT32F;
	texture_data.height = 2048;
	texture_data.width = 2048;

	auto format = Core::TextureFormat::create_texture2d_format(false);
	format->set_parameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	format->set_parameter(GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	shadow_map = Core::TextureFactory::create(*format, texture_data);

	texture_manager->add(shadow_map_name, shadow_map);
}

SimpleShadowMap::~SimpleShadowMap()
{
}

void SimpleShadowMap::build()
{
	framebuffer = std::make_shared<Core::FramebufferObject>();
	framebuffer->bind();
	framebuffer->add(GL_DEPTH_ATTACHMENT, shadow_map);
	framebuffer->set_draw_buffer_as_map({});
	glClearDepth(1.0f);
	framebuffer->check();
	framebuffer->unbind();

	camera = owner->get_component<GM::Framework::Camera>();
}

void SimpleShadowMap::pass(Framework::RenderSystem &render_system)
{
	framebuffer->bind();
	glViewportIndexedf(0, 0, 0, 2048, 2048);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	render_system.pass(*camera, "shadow", (1<<10)-1);

	glCullFace(GL_BACK);
	framebuffer->unbind();
}

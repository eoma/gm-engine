#include "../include/CommonComponentSerializer.h"
#include "../include/CommonDefinitionsPropertyNames.h"

#include "../include/FpsController.h"
#include "../include/IdleRotate.h"
#include "../include/Starfield.h"

using namespace GM;
using namespace Samples;

GM::Common::CommonComponentSerializer::CommonComponentSerializer(const Application::MainPtr &app) : app(app.get()) {
	slots.connect(
		app->get_entity_manager()->register_component_serializer_signal(),
		this, &CommonComponentSerializer::create_and_add_component);
}

void GM::Common::CommonComponentSerializer::create_and_add_component(const Framework::EntityPtr &owner, const std::string &type, const std::string &/*name*/) {
	if (type == Samples::FPSControllerComponent::get_static_type()) {
		owner->create_component<Samples::FPSControllerComponent>(app);
	}
	else if (type == Samples::IdleRotationComponent::get_static_type()) {
		owner->create_component<Samples::IdleRotationComponent>();
	}
	else if (type == Samples::StarfieldComponent::get_static_type()) {
		owner->create_component<Samples::StarfieldComponent>(app);
	}
}

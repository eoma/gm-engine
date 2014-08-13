#include "SamplesComponentSerializer.h"
#include "GM/Samples/SampleDefinitionsPropertyNames.h"

#include "GM/Samples/FpsController.h"
#include "GM/Samples/IdleRotate.h"

using namespace GM;
using namespace Samples;

SamplesComponentSerializer::SamplesComponentSerializer(const Application::MainPtr &app) : app(app.get()) {
	slots.connect(
		app->get_entity_manager()->register_component_serializer_signal(),
		this, &SamplesComponentSerializer::create_and_add_component);
}

void SamplesComponentSerializer::create_and_add_component(const Framework::EntityPtr &owner, const std::string &type, const std::string &/*name*/) {
	if (type == Samples::FPSControllerComponent::get_static_type()) {
		owner->create_component<Samples::FPSControllerComponent>(app);
	}
	else if (type == Samples::IdleRotationComponent::get_static_type()) {
		owner->create_component<Samples::IdleRotationComponent>();
	}
}

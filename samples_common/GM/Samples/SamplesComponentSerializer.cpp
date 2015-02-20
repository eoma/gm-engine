#include "SamplesComponentSerializer.h"
#include "GM/Samples/SampleDefinitionsPropertyNames.h"

#include "GM/Samples/FpsController.h"
#include "GM/Samples/IdleRotate.h"
#include "GM/Samples/Starfield.h"

#include "GM/Application/Main.h"
#include "GM/Application/MainComponentSerializer.h"

using namespace GM;
using namespace Samples;

SamplesComponentSerializer::SamplesComponentSerializer(const Application::MainPtr &app) {
	app->get_component_serializer()->add_component_creator(Samples::FPSControllerComponent::get_static_type(),
		[app](const Framework::EntityPtr &owner) {
			owner->create_component<Samples::FPSControllerComponent>(app.get());
		}
	);

	app->get_component_serializer()->add_component_creator(Samples::IdleRotationComponent::get_static_type(),
		[app](const Framework::EntityPtr &owner) {
			owner->create_component<Samples::IdleRotationComponent>();
		}
	);
	
	app->get_component_serializer()->add_component_creator(Samples::StarfieldComponent::get_static_type(),
		[app](const Framework::EntityPtr &owner) {
			owner->create_component<Samples::StarfieldComponent>(app.get());
		}
	);
}

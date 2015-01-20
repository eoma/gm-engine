#pragma once

#include <ClanLib/core.h>

#include "../Totem/Totem.h"

#include "GL/gl3w.h"

#include <string>
#include <memory>

namespace GM {
	namespace Core {
		class Shader; typedef std::shared_ptr<Shader> ShaderPtr;
	} // namespace Core

	namespace Framework {

		/**
		 * Connects properties with uniforms in a given shader.
		 * Will usually connect only one property per uniform (this can be overridden).
		 */
		class PropertyToUniformConnector {
		public:
			PropertyToUniformConnector(const Core::ShaderPtr &shader = nullptr);
			~PropertyToUniformConnector();

			/**
			 * Perform uniform updates for connected properties
			 */
			void update_uniforms();

			/**
			 * Connect a single property to a (possibly) equal named uniform in the
			 * associated shader. You can override property name and force ot to connect
			 * with a specific uniform, given they have the same type.			 *
			 */
			void connect(const IProperty &property, const std::string &override_name = std::string(), bool force_connection = false);

			/**
			 * Connect all properties in a property container. Will also create and connect 
			 * properties present in the shader, but not in the container.
			 */
			void connect_property_container(PropertyContainer<> &property_container);

			/**
			 * Creates an instance with a copy of used_uniforms,
			 * current_available_texture_unit and uniform_to_texture_units.
			 *
			 * Useful when you want to create a base setup (like in a material) and
			 * want to avoid overrides (eg. users of a material), but still allow usage
			 * of unused uniform values.
			 */
			PropertyToUniformConnector create_derivative() const;

			const Core::ShaderPtr &get_shader() const { return shader; }

			bool uniform_is_used(const std::string &name) const;

		private:
			/**
			 * Confirms that base_property is an instance of Property<ValueType> and
			 * generates and registers a uniform update function related to the
			 * property instance
			 */
			template <class ValueType>
			void connect_property_with_type(const IProperty &base_property, const int uniform_location);

			/**
			 * Creates a lambda function responsible for updating a uniform.
			 * When triggere the function will fetch the data in the property and
			 * call the appropriate glUniform*() function.
			 */
			template <class ValueType>
			std::function<void()> create_uniform_update_function(const Property<ValueType> &property, const int uniform_location);

			int get_available_texture_unit();

			// Helper classes intended to be used within implementation
			class CallConnectPropertyWithType;
			class ConstructPropertyThenConnect;

		private:
			Core::ShaderPtr shader;
			clan::SlotContainer uniform_slots;
			clan::Signal<void()> update_uniforms_signal;

			int current_available_texture_unit;

			std::vector<std::string> used_uniforms;
			std::map<int, int> uniform_to_texture_units;
		};

	} // namespace Framework
} // namespace GM

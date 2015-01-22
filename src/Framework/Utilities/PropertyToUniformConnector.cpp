#include "GM/Framework/Utilities/PropertyToUniformConnector.h"

#include "GM/Core/GL/Shader.h"
#include "GM/Core/GL/Texture.h"
#include "GM/Core/Utilities/UniformOperations.h"

#include "glm/glm.hpp"

#include <algorithm>

namespace GM {
	namespace Framework {

		namespace {
			/**
			 * A special method to determine glsl type and then call a class method with 
			 * the appropriate type (usually one glm's) as a template parameter.
			 *
			 * The SomeClassType template type _must_ have a _templated_ method called void apply()
			 * Your apply method can accept parameters, but they must be supplied on call of determine_type_and_apply.
			 *
			 * The following minimal class will accept the minimal requirement:
			 * class SomeClassImplementation {
			 * public:
			 *     template<class T> void apply() {}
			 * };
			 *
			 * The following will accept an argument:
			 * class SomeImpl {
			 * public:
			 *     template<T> void apply(int a) { some_operations... }
			 * };
			 *
			 * and should be called as (eg.):
			 *  determine_type_and_apply<SomeImpl>(info, 10);
			 */
			template<class SomeClassType, typename... ApplyArgumentTypes>
			bool determine_type_and_apply(const Core::ShaderVariableInfo &info, ApplyArgumentTypes&&... arguments);
		} // unnamed / anonymous namespace

		PropertyToUniformConnector::PropertyToUniformConnector(const Core::ShaderPtr &shader)
		: shader(shader)
		, current_available_texture_unit(0)
		{
		}

		PropertyToUniformConnector::~PropertyToUniformConnector()
		{
		}

		void PropertyToUniformConnector::update_uniforms()
		{
			update_uniforms_signal();
		}

		PropertyToUniformConnector PropertyToUniformConnector::create_derivative() const
		{
			PropertyToUniformConnector instance(shader);

			instance.current_available_texture_unit = current_available_texture_unit;
			instance.used_uniforms = used_uniforms;
			instance.uniform_to_texture_units = uniform_to_texture_units;

			return instance;
		}

		bool PropertyToUniformConnector::uniform_is_used(const std::string &name) const
		{
			return std::find(used_uniforms.begin(), used_uniforms.end(), name) != used_uniforms.end();
		}

		// Helper class for connect_property, primarily used by determine_type_and_apply
		class PropertyToUniformConnector::CallConnectPropertyWithType {
		public:
			template <class T>
			void apply(PropertyToUniformConnector &inst, const IProperty &property, int uniform_location) {
				inst.connect_property_with_type<T>(property, uniform_location);
			}

		};

		void PropertyToUniformConnector::connect(const IProperty &property, const std::string &override_name, bool force_connection)
		{
			if (shader == nullptr)
			{
				return;
			}

			std::string name("");
			if (!override_name.empty())
			{
				name = override_name;
			}
			else if (!property.get_name().empty())
			{
				name = property.get_name();
			}

			if (name.empty())
			{
				throw clan::Exception("A property connected to a uniform must have a name!");
			}

			bool already_setup = uniform_is_used(name);

			if (!shader->contains_uniform(name) || (already_setup && !force_connection))
			{
				return;
			}

			Core::ShaderVariableInfo info = shader->get_uniform_info(name);

			std::cout << "Will connect " << info.name << std::endl;

			bool successfully_connected_property = determine_type_and_apply<CallConnectPropertyWithType>(info, *this, property, info.location);

			if (!already_setup && successfully_connected_property) {
				used_uniforms.push_back(name);
			}
		}

		// To be used in conjunction with determine_type_and_apply()
		class PropertyToUniformConnector::ConstructPropertyThenConnect {
		public:
			template<class T>
			void apply(PropertyToUniformConnector &inst, PropertyContainer<> &property_container, const Core::ShaderVariableInfo &info) {

					if (property_container.has_property(info.name)) {
						inst.connect(property_container.get<T>(info.name));
					} else {
						Property<T> prop = property_container.add<T>(info.name, T());
						inst.connect(prop);
					}
			}
		};

		void PropertyToUniformConnector::connect_property_container(PropertyContainer<> &property_container)
		{
			if (shader == nullptr)
			{
				return;
			}

			std::cout << "Add uniforms for property container!" << std::endl;

			for (Core::ShaderVariableInfo info : shader->get_uniform_infos())
			{
				determine_type_and_apply<ConstructPropertyThenConnect>(info, *this, property_container, info);
			}
		}

		template <class ValueType>
		void PropertyToUniformConnector::connect_property_with_type(const IProperty &base_property, const int uniform_location)
		{
			if (base_property.get_runtime_type_id() != IProperty::get_runtime_type_id<ValueType>())
			{
				throw clan::Exception(clan::string_format("Property is not of correct type, expected it to be %1!", typeid(ValueType).name()));
			}

			const Property<ValueType>& property = static_cast<const Property<ValueType>&>(base_property);
			uniform_slots.connect(update_uniforms_signal, create_uniform_update_function<ValueType>(property, uniform_location));
		}


		// Template specialization of uniform_maker for textures
		template <>
		std::function<void()> PropertyToUniformConnector::create_uniform_update_function<Core::TexturePtr>(const Property<Core::TexturePtr> &property, const int uniform_location)
		{
			unsigned int program = shader->get_handle();


			// Try to reuse texture units for same uniform location
			// in the same shader
			auto iter = uniform_to_texture_units.find(uniform_location);
			if (iter == uniform_to_texture_units.end())
			{
				iter = uniform_to_texture_units.emplace(std::make_pair(uniform_location, get_available_texture_unit())).first;
			}

			int texture_unit = iter->second;
			
			std::cout << "Set up uniform texture " << property.get_name() << " with texture unit " << texture_unit << std::endl;

			return [program, uniform_location, property, texture_unit]() {
				if (property.get() == nullptr)
				{
					return;
				}

				glActiveTexture(GL_TEXTURE0 + texture_unit);
				property.get()->bind();

				Core::update_uniform(program, uniform_location, texture_unit);
			};
		}

		template <class ValueType>
		std::function<void()> PropertyToUniformConnector::create_uniform_update_function(const Property<ValueType> &property, const int uniform_location)
		{
			std::cout << "Set up uniform " << property.get_name() << std::endl;

			unsigned int program = shader->get_handle();
			return [program, uniform_location, property]() {
				Core::update_uniform(program, uniform_location, property.get());
			};
		}

		int PropertyToUniformConnector::get_available_texture_unit()
		{
			return current_available_texture_unit++;
		}

		namespace {
			template<class SomeClassType, typename... ApplyArgumentTypes>
			bool determine_type_and_apply(const Core::ShaderVariableInfo &info, ApplyArgumentTypes&&... arguments)
			{
				bool successful = true;
	
				// TODO: add more types from here: https://www.opengl.org/wiki/GLAPI/glGetActiveUniforms#Description
				switch (info.type)
				{
				case GL_BOOL:
					{
						SomeClassType().template apply<bool>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
	
				case GL_BYTE:
					{
						SomeClassType().template apply<char>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_UNSIGNED_BYTE:
					{
						SomeClassType().template apply<unsigned char>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_SHORT:
					{
						SomeClassType().template apply<short>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_UNSIGNED_SHORT:
					{
						SomeClassType().template apply<unsigned short>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_INT:
					{
						SomeClassType().template apply<int>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_UNSIGNED_INT:
					{
						SomeClassType().template apply<unsigned int>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_FLOAT:
					{
						SomeClassType().template apply<float>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_FLOAT_VEC2:
					{
						SomeClassType().template apply<glm::vec2>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_INT_VEC2:
					{
						SomeClassType().template apply<glm::ivec2>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_UNSIGNED_INT_VEC2:
					{
						SomeClassType().template apply<glm::uvec2>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_FLOAT_VEC3:
					{
						SomeClassType().template apply<glm::vec3>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_INT_VEC3:
					{
						SomeClassType().template apply<glm::ivec3>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_UNSIGNED_INT_VEC3:
					{
						SomeClassType().template apply<glm::uvec3>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_FLOAT_VEC4:
					{
						SomeClassType().template apply<glm::vec4>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_INT_VEC4:
					{
						SomeClassType().template apply<glm::ivec4>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_UNSIGNED_INT_VEC4:
					{
						SomeClassType().template apply<glm::uvec4>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_FLOAT_MAT2:
					{
						SomeClassType().template apply<glm::mat2>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_FLOAT_MAT3:
					{
						SomeClassType().template apply<glm::mat3>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_FLOAT_MAT4:
					{
						SomeClassType().template apply<glm::mat4>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				case GL_SAMPLER_2D:
				case GL_SAMPLER_CUBE:
				case GL_SAMPLER_2D_SHADOW:
				case GL_SAMPLER_CUBE_SHADOW:
					{
						SomeClassType().template apply<Core::TexturePtr>(std::forward<ApplyArgumentTypes>(arguments)...);
						break;
					}
		
				default:
					{
						successful = false;
	
						clan::StringFormat message("Uknown/unhandled type (%1) for uniform (%2) with length (%3)");
						message.set_arg(1, info.type);
						message.set_arg(2, info.name);
						message.set_arg(3, info.size);
		
						std::cerr << message.get_result();
						break;
					}
		
				}
	
				return successful;
			}
		} // unnamed / hidden / anonymous namespace
	} //namespace Framework
} // namespace GM

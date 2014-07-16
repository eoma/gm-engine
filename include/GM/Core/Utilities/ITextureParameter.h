#pragma once

#include <memory>

namespace GM {
	namespace Core {
		class ITextureParameter
		{
		protected:
			unsigned int param_name;

		public:
			ITextureParameter(const unsigned int param_name) : param_name(param_name) {};

			unsigned int get_name() const { return param_name; }

			static int compare(const ITextureParameter &current, const ITextureParameter &other)
			{
				int result = current.param_name - other.param_name;
				
				if (result == 0) result = (int)(current.get_runtime_type_id() - other.get_runtime_type_id());
				if (result == 0) result = current.compare_param(other); // types should be equal now

				return result;
			}

			template<class T> static bool is_type(const std::shared_ptr<ITextureParameter> &parameter);
			template<class T> static bool is_type(const ITextureParameter &parameter);
			virtual unsigned int get_runtime_type_id() const = 0;
			template<class T> static unsigned int get_runtime_type_id();

		protected:
			// Compares two parameter values if they are of equal type.
			virtual int compare_param(const ITextureParameter &other) const = 0;
		};

		typedef std::shared_ptr<ITextureParameter> ITextureParameterPtr;

		template<class T>
		bool ITextureParameter::is_type(const std::shared_ptr<ITextureParameter> &parameter)
		{
			return (parameter->get_runtime_type_id() == get_runtime_type_id<T>());
		}

		template<class T>
		bool ITextureParameter::is_type(const ITextureParameter &parameter)
		{
			return (parameter.get_runtime_type_id() == get_runtime_type_id<T>());
		}

		template<class T>
		unsigned int ITextureParameter::get_runtime_type_id()
		{
			static unsigned int type_id(typeid(T).hash_code());
			return type_id;
		}

		bool operator< (const ITextureParameter &current, const ITextureParameter &other);

	} // namespace Core
} // namespace GM

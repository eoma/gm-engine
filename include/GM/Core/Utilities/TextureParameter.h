#pragma once

#include "ITextureParameter.h"

namespace GM {
namespace Core {

/**
 * This is just a holder of parameter name and value.
 */
template<class T>
class TextureParameter;

typedef TextureParameter<int> TextureParameterInt;
typedef TextureParameter<float> TextureParameterFloat;

template<class T>
class TextureParameter : ITextureParameter
{
private:
	T param;

public:
	TextureParameter(const unsigned int param_name, const T param) : ITextureParameter(param_name), param(param) {};

	const T &get_param() const { return param; }
	void set_param(const T &param) { this->param = param; }

	static int compare(const TextureParameter<T> &current, const TextureParameter<T> &other)
	{
		int result = ITextureParameter::compare(current, other);

		if (result == 0) result = (int)(current.param - other.param);

		return result;
	}

	unsigned int get_runtime_type_id() const override;
};

template<class T>
unsigned int TextureParameter<T>::get_runtime_type_id() const
{
	return ITextureParameter::template get_runtime_type_id<T>();
}

template <class T>
bool operator< (const TextureParameter<T> &current, const TextureParameter<T> &other)
{
	return TextureParameter<T>::compare(current, other) < 0;
}

} // namespace Core
} // namespace GM

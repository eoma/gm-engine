#pragma once

#include "ITextureParameter.h"
#include "Tools.h"

#include <glm/glm.hpp>
#include <vector>

namespace GM {
namespace Core {

/**
 * This is just a holder of parameter name and value.
 */
template<class T>
class TextureParameter;

typedef TextureParameter<int> TextureParameterInt;
typedef std::shared_ptr<TextureParameterInt> TextureParameterIntPtr;

typedef TextureParameter<glm::ivec4> TextureParameterIVec4;
typedef std::shared_ptr<TextureParameterIVec4> TextureParameterIVec4Ptr;

typedef TextureParameter<float> TextureParameterFloat;
typedef std::shared_ptr<TextureParameterFloat> TextureParameterFloatPtr;

typedef TextureParameter<glm::vec4> TextureParameterVec4;
typedef std::shared_ptr<TextureParameterVec4> TextureParameterVec4Ptr;

template<class T>
class TextureParameter : public ITextureParameter
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
		return result;
	}

	unsigned int get_runtime_type_id() const override;

protected:
	int compare_param(const ITextureParameter &other) const override;
};

template<class T>
unsigned int TextureParameter<T>::get_runtime_type_id() const
{
	return ITextureParameter::template get_runtime_type_id<T>();
}

template<class T>
int TextureParameter<T>::compare_param(const ITextureParameter &other) const
{
	// Should only be called if other is of same type as us
	int result = 0;

	// May be skipped if we can guarantee that other is _always_ of the same type as us
	if (!ITextureParameter::is_type<T>(other)) {
		result = (int)(get_runtime_type_id() - other.get_runtime_type_id());
	}
	else
	{
		const T &other_param = static_cast<const TextureParameter<T>&>(other).get_param();

		// Safe comparison using relational algebra. STL implements relation algebra for vector, set, ...
		result = param < other_param ? -1 : (other_param < param ? 1 : 0); // if result is 0, they should be equal
	}

	return result;
}

template <class T>
bool operator< (const TextureParameter<T> &current, const TextureParameter<T> &other)
{
	return TextureParameter<T>::compare(current, other) < 0;
}

} // namespace Core
} // namespace GM

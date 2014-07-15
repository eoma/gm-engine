#pragma once

namespace GM {
namespace Core {

/**
 * This is just a holder of parameter name and value.
 */
template<class T>
struct TexParameter;

typedef TexParameter<int> TexParameterInt;
typedef TexParameter<float> TexParameterFloat;

template<class T>
struct TexParameter
{
	unsigned int param_name;
	T param;

	TexParameter(const unsigned int param_name, const T param) : param_name(param_name), param(param) {};

	static int compare(const TexParameter<T> &current, const TexParameter<T> &other)
	{
		int result = current.param_name - other.param_name;

		if (result == 0) result = (int)(current.param - other.param);

		return result;
	}
};

template <class T>
bool operator< (const TexParameter<T> &current, const TexParameter<T> &other)
{
	return TexParameter<T>::compare(current, other) < 0;
}

} // namespace Core
} // namespace GM

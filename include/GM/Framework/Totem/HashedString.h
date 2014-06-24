#pragma once
#include <ClanLib/core.h>
 
#include <functional>

//This class is based on: http://www.altdevblogaday.com/2011/10/27/quasi-compile-time-string-hashing/

#ifdef WIN32
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE inline
#endif

namespace GM {
namespace Framework {

template <unsigned int N, unsigned int I>
struct HashFNV1a
{
        FORCE_INLINE static unsigned int hash(const char (&str)[N])
        {
                return (HashFNV1a<N, I-1>::hash(str) ^ str[I-1])*16777619u;
        }
};

template <unsigned int N>
struct HashFNV1a<N, 1>
{
        FORCE_INLINE static unsigned int hash(const char (&str)[N])
        {
                return (2166136261u ^ str[0])*16777619u;
        }
};

class HashedString
{
public:
	template<unsigned int N>
	FORCE_INLINE HashedString(const char (&value)[N])
		: hash_id(HashFNV1a<N,N>::hash(value)), str(value)
	{
	}

	FORCE_INLINE unsigned int get_id() const { return hash_id; }
	FORCE_INLINE const std::string &get_str() const { return str; }

private:
	unsigned int hash_id;
	std::string str;
};

} // namespace Framework
} // namespace GM

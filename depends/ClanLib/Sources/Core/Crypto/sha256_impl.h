/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Mark Page
*/

#pragma once

#include "sha.h"

namespace clan
{

class SHA256_Impl : private SHA
{
/// \name Construction
/// \{

public:
	SHA256_Impl(cl_sha_type new_sha_type);


/// \}
/// \name Attributes
/// \{

public:
	std::string get_hash(bool uppercase = false) const;

	void get_hash(unsigned char *out_hash) const;


/// \}
/// \name Operations
/// \{

public:
	void reset();
	void set_hmac(const void *key_data, int key_size);

	void add(const void *data, int size);

	void calculate();


/// \}
/// \name Implementation
/// \{

private:
	inline ubyte32 sigma_rr2_rr13_rr22(ubyte32 value) const
	{
		return (rightrotate_uint32(value, 2) ^ rightrotate_uint32(value, 13) ^ rightrotate_uint32(value, 22));
	}

	inline ubyte32 sigma_rr6_rr11_rr25(ubyte32 value) const
	{
		return (rightrotate_uint32(value, 6) ^ rightrotate_uint32(value, 11) ^ rightrotate_uint32(value, 25));
	}

	inline ubyte32 sigma_rr7_rr18_sr3(ubyte32 value) const
	{
		return (rightrotate_uint32(value, 7) ^ rightrotate_uint32(value, 18) ^ (value >> 3));
	}

	inline ubyte32 sigma_rr17_rr19_sr10(ubyte32 value) const
	{
		return (rightrotate_uint32(value, 17) ^ rightrotate_uint32(value, 19) ^ (value >> 10));
	}

	inline ubyte32 sha_ch(ubyte32 x, ubyte32 y, ubyte32 z) const
	{
		return (((x) & ((y) ^ (z))) ^ (z));
	}

	inline ubyte32 sha_maj(ubyte32 x, ubyte32 y, ubyte32 z) const
	{
		return  (((x) & ((y) | (z))) | ((y) & (z)));
	}

	void process_chunk();

	ubyte32 h0, h1, h2, h3, h4, h5, h6, h7;

	const static int block_size = 64;

	unsigned char chunk[block_size];

	int chunk_filled;

	ubyte64 length_message;

	bool calculated;

	cl_sha_type sha_type;
	bool hmac_enabled;
	unsigned char hmac_key_chunk[block_size];
/// \}
};

}

#pragma once

#include <cstddef>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <memory>
#include <iostream>

namespace GM {
	namespace Core {
		class Shader; typedef std::shared_ptr<Shader> ShaderPtr;

		//
		// End user function declarations
		//

		void print_shader_info(const ShaderPtr &shader, std::ostream &out = std::cout);

		// Lexicographical less than operator for glm::tvec4, for tvec{1,3} just copy and replace 4
		template <typename T, glm::precision P>
		bool operator< (const glm::tvec4<T,P> &v0, const glm::tvec4<T,P> &v1) {
			return std::lexicographical_compare(glm::value_ptr(v0), glm::value_ptr(v0) + v0.length(),
				glm::value_ptr(v1), glm::value_ptr(v1) + v1.length());
		}

		template <class... DataStructures>
		size_t total_size(const DataStructures&... data_structures);

		// Almost the same as total_size, except it will add one extra to each containers' size
		template <class... DataStructures>
		size_t total_size_plus_one(const DataStructures&... data_structures);

		//
		// Internal implementation declarations
		//

		template <class First, class... Rest>
		size_t total_size_impl(const size_t extra_count, size_t accumulated, const First &first, const Rest&... rest);

		template <class Single>
		size_t total_size_impl(const size_t extra_count, size_t accumulated, const std::vector<Single> &single);

		//
		// Implementations
		//

		template <class... datastructures>
		size_t total_size(const datastructures&... data_structures)
		{
			return total_size_impl(0, 0, data_structures...);
		}

		template <class... datastructures>
		size_t total_size_plus_one(const datastructures&... data_structures)
		{
			return total_size_impl(1, 0, data_structures...);
		}
		
		template <class First, class... Rest>
		size_t total_size_impl(const unsigned int extra_count, size_t accumulated, const First &first, const Rest&... rest)
		{
			return total_size_impl(
				extra_count,
				total_size_impl(extra_count, accumulated, first),
				rest...);
		}

		template <class Single>
		size_t total_size_impl(const size_t extra_count, size_t accumulated, const std::vector<Single> &single)
		{
			return accumulated + sizeof(Single)*(single.size()+extra_count);
		}

	} // namespace Core
} // namespace GM

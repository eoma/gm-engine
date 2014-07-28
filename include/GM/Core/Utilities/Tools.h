#pragma once

#include <cstddef>
#include <vector>

namespace GM {
	namespace Core {
		
		//
		// End user function declarations
		//

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

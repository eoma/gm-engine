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

		//
		// Internal implementation declarations
		//

		template <class First, class... Rest>
		size_t total_size_impl(size_t accumulated, const First &first, const Rest&... rest);

		template <class Single>
		size_t total_size_impl(size_t accumulated, const std::vector<Single> &single);

		//
		// Implementations
		//

		template <class... datastructures>
		size_t total_size(const datastructures&... data_structures)
		{
			return total_size_impl(0, data_structures...);
		}

		
		template <class First, class... Rest>
		size_t total_size_impl(size_t accumulated, const First &first, const Rest&... rest)
		{
			return total_size_impl(total_size_impl(accumulated, first), rest...);
		}

		template <class Single>
		size_t total_size_impl(size_t accumulated, const std::vector<Single> &single)
		{
			return accumulated + sizeof(Single)*single.size();
		}

	} // namespace Core
} // namespace GM

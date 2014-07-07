#pragma once

#include <algorithm>
#include <set>
#include <string>
#include <vector>

namespace GM {
	namespace Framework {
		/**
		 * Locates a path based on path hierarchy. Will only search upward in the hierarchy.
		 *
		 * @param start_path The starting point, eg. the path to your executable.
		 * @param wanted_end_of_path what the resulting path must contain.
		 * @param max_ancestor_length how many steps up the directory hierarchy are allowed to search.
		 * @return the found path
		 * @throws std::runtime_error if path is not found.
		 */
		std::string find_path_in_hierarchy(const std::string &start_path, const std::string &wanted_end_of_path, const unsigned int max_ancestor_length = 100);

		template<class T>
		std::set<T> general_set_intersection(const std::vector<std::set<T>> &sets) {
			std::vector<T> result;

			if (sets.size() > 0)
			{
				result.assign(sets[0].begin(), sets[0].end());

				std::vector<T> temp;
				for (auto iter = sets.begin()+1; iter != sets.end(); ++iter)
				{
					temp.clear();
					temp.reserve(result.size() + iter->size());

					auto end = std::set_intersection(result.begin(), result.end(), iter->begin(), iter->end(), temp.begin());

					result.clear();
					result.assign(temp.begin(), end);

					if (result.size() == 0)
					{
						break;
					}
				}
			}

			return std::set<T>(result.begin(), result.end());
		}
	}
}

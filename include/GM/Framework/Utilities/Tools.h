#pragma once

#include <string>

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
	}
}

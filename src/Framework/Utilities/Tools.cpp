#include "GM/Framework/Utilities/Tools.h"

#include <ClanLib/core.h>

std::string GM::Framework::find_path_in_hierarchy(const std::string &start_path, const std::string &wanted_end_of_path, const unsigned int max_ancestor_length) {
	bool found = false;
	std::string root_path = start_path;
	std::string path = clan::PathHelp::make_absolute(root_path, wanted_end_of_path);

	// We use the directory scanner to determine if a folder can be scanned
	// and thereby determine whether it exists or not.
	clan::DirectoryScanner directory_scanner;

	for (unsigned int i = 0; !found && i < max_ancestor_length; ++i) {
		found = directory_scanner.scan(path);

		if (!found) {
			try {
				root_path = clan::PathHelp::make_absolute(root_path, "../");
			} catch (clan::Exception e) {
				// We are unable to get further down the hiearchy, break out of loop
				break;
			}

			path = clan::PathHelp::make_absolute(root_path, wanted_end_of_path);
		}
	}

	if (!found) {
		throw std::runtime_error("Unable to find path");
	}
	
	return path;
}

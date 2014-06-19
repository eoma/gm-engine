#include <cstdlib>


bool mainTest() {
	return true;
}

int main() {
	
	bool successful = mainTest();

	if (successful) {
		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}
}

cmake_minimum_required(VERSION 2.8)

set(NAME TriangleSample)
project(${NAME})

file(GLOB_RECURSE Headers ${CMAKE_CURRENT_LIST_DIR}/*.h)
file(GLOB_RECURSE Sources ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

if ( Headers OR Sources )
	add_executable(${NAME} ${Headers} ${Sources})
	target_link_libraries(${NAME}
		Application
		Framework
		Core) # or something else

	set_target_properties(${NAME} PROPERTIES
		FOLDER "Samples"
		RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/samples)
else()
	message("No source files defined for test ${NAME}")
endif()

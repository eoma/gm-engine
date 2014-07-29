cmake_minimum_required(VERSION 2.8)

set(NAME RenderLayerTest)
project(${NAME})

file(GLOB_RECURSE Headers ${CMAKE_CURRENT_LIST_DIR}/*.h)
file(GLOB_RECURSE Sources ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

if ( Headers OR Sources )
	#add_excutable(${NAME} main.cpp)
	add_executable(${NAME} ${Headers} ${Sources})
	target_link_libraries(${NAME}
		Framework
		Core
		SOIL2
		gl3w
		glfw
		${GLFW_LIBRARIES}
		${CMAKE_DL_LIBS}) # or something else

	set_target_properties(${NAME} PROPERTIES
		FOLDER "Tests"
		RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/tests)

	add_test(NAME ${NAME} COMMAND ${NAME})
else()
	message("No source files defined for test ${NAME}")
endif()

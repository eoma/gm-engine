cmake_minimum_required(VERSION 2.8)

set(NAME JsonTemplateTest)
project(${NAME})

file(GLOB_RECURSE Headers ${CMAKE_CURRENT_LIST_DIR}/*.h)
file(GLOB_RECURSE Sources ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

file(GLOB_RECURSE JSON ${CMAKE_SOURCE_DIR}/resources/json/tests/*.json)
source_group("JSON Files\\" FILES ${JSON})

if ( Headers OR Sources )
	#add_excutable(${NAME} main.cpp)
	add_executable(${NAME} ${Headers} ${Sources} ${JSON})
	target_link_libraries(${NAME}
		Framework
		Core) # or something else

	set_target_properties(${NAME} PROPERTIES
		FOLDER "Tests"
		RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/tests)

	add_test(NAME ${NAME} COMMAND ${NAME})
else()
	message("No source files defined for test ${NAME}")
endif()

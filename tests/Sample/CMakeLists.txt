cmake_minimum_required(VERSION 2.8)

set(NAME Sample)
project(${NAME})

file(GLOB_RECURSE Headers ${CMAKE_CURRENT_LIST_DIR}/*.h)
file(GLOB_RECURSE Sources ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

if ( Headers OR Sources )
	#add_excutable(${NAME} main.cpp)
	add_executable(${NAME} ${Headers} ${Sources})
	target_link_libraries(${NAME} Framework) # or something else
	
	include_directories(
	  ${CMAKE_SOURCE_DIR}/include/
	  ${CMAKE_SOURCE_DIR}/depends/ClanLib/Include/
	  ${CMAKE_SOURCE_DIR}/depends/glm/include/
	  ${CMAKE_SOURCE_DIR}/depends/totem-edk/include/
	)

	set_target_properties(${NAME} PROPERTIES
		FOLDER "Tests"
		RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/tests)

	add_test(NAME ${NAME} COMMAND ${NAME})
else()
	message("No source files defined for test ${NAME}")
endif()

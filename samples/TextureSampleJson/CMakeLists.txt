cmake_minimum_required(VERSION 2.8)

set(NAME TextureSampleJson)
project(${NAME})

file(GLOB_RECURSE Headers ${CMAKE_CURRENT_LIST_DIR}/*.h)
file(GLOB_RECURSE Sources ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

file(GLOB_RECURSE JSON ${CMAKE_SOURCE_DIR}/resources/json/samples/texture/*.json)
set(GLSL_Path ${CMAKE_SOURCE_DIR}/resources/glsl/samples/texture/)
file(GLOB_RECURSE GLSL ${GLSL_Path}*.vs ${GLSL_Path}*.fs ${GLSL_Path}*.gs ${GLSL_Path}*.cs ${GLSL_Path}*.tcs ${GLSL_Path}*.tes)
source_group("JSON Files\\" FILES ${JSON})
source_group("GLSL Files\\" FILES ${GLSL})

if ( Headers OR Sources )
	add_executable(${NAME} ${Headers} ${Sources} ${JSON} ${GLSL})
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

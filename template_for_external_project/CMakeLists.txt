cmake_minimum_required(VERSION 2.8)

project(ExternalProjectTemplate)

find_package(GmEngine)
include(${GM_ENGINE_COMPILER_SETUP_SCRIPT})

include_directories(${GM_ENGINE_INCLUDE_DIRS})
add_definitions(${GM_ENGINE_DEFINITIONS})

file(GLOB_RECURSE Headers ${PROJECT_SOURCE_DIR}/inlude/*.h)
file(GLOB_RECURSE Sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
source_group("Header Files\\" FILES ${Headers})
source_group("Source Files\\" FILES ${Sources})

file(GLOB_RECURSE JSON ${PROJECT_SOURCE_DIR}/resources/json/*.json)
set(GLSL_Path ${PROJECT_SOURCE_DIR}/resources/glsl)
file(GLOB_RECURSE GLSL ${GLSL_Path}*.vs ${GLSL_Path}*.fs ${GLSL_Path}*.gs ${GLSL_Path}*.cs ${GLSL_Path}*.tcs ${GLSL_Path}*.tes)
source_group("JSON Files\\" FILES ${JSON})
source_group("GLSL Files\\" FILES ${GLSL})

if ( Headers OR Sources )
	add_executable(${PROJECT_NAME} ${Headers} ${Sources} ${JSON} ${GLSL})
	target_link_libraries(${PROJECT_NAME} ${GM_ENGINE_LIBRARIES}) # or something else

	set_target_properties(${PROJECT_NAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
else()
	message("No source files defined for ${PROJECT_NAME}")
endif()

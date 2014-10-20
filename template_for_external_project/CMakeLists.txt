cmake_minimum_required(VERSION 2.8)

project(ExternalProjectTemplate)

find_package(GmEngine)
include(${GM_ENGINE_COMPILER_SETUP_SCRIPT})

include_directories(${GM_ENGINE_INCLUDE_DIRS})
add_definitions(${GM_ENGINE_DEFINITIONS})

#
# Locate local source files
#

file(GLOB_RECURSE Headers ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE Sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
source_group("Header Files\\" FILES ${Headers})
source_group("Source Files\\" FILES ${Sources})

file(GLOB_RECURSE CommonHeaders ${PROJECT_SOURCE_DIR}/common/include/*.h)
file(GLOB_RECURSE CommonSources ${PROJECT_SOURCE_DIR}/common/src/*.cpp)
source_group("Header Files\\Common\\" FILES ${CommonHeaders})
source_group("Source Files\\Common\\" FILES ${CommonSources})

#
# Locate json and glsl files
#
file(GLOB_RECURSE JSON ${PROJECT_SOURCE_DIR}/resources/json/*.json)
set(GLSL_Path ${PROJECT_SOURCE_DIR}/resources/glsl)

file(GLOB_RECURSE GLSL ${GLSL_Path}*.vs ${GLSL_Path}*.fs ${GLSL_Path}*.gs ${GLSL_Path}*.cs
	${GLSL_Path}*.tcs ${GLSL_Path}*.tes ${GLSL_Path}*.glsl)

source_group("JSON Files\\" FILES ${JSON})
source_group("GLSL Files\\" FILES ${GLSL})

#
# Set project specific properties
#

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/common/include)

if ( Headers OR Sources )
	add_executable(${PROJECT_NAME} ${Headers} ${Sources} ${CommonHeaders} ${CommonSources} ${JSON} ${GLSL})
	target_link_libraries(${PROJECT_NAME} ${GM_ENGINE_LIBRARIES}) # or something else

	set_target_properties(${PROJECT_NAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

	# Uses the GM_ENGINE_RESOURCE_DIR variable that gets set when we run find_package(GmEngine)
	configure_file(external_resource_paths.json.in
		"${CMAKE_SOURCE_DIR}/bin/external_resource_paths.json" @ONLY)
else()
	message("No source files defined for ${PROJECT_NAME}")
endif()

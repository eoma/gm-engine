cmake_minimum_required(VERSION 2.8)

# Framework requirements
include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/depends/glm/include
	${CMAKE_SOURCE_DIR}/depends/ClanLib/Include
	${CMAKE_SOURCE_DIR}/depends/SOIL2/include
	${CMAKE_SOURCE_DIR}/depends/LodePNG/include
	${CMAKE_SOURCE_DIR}/depends/gl3w/include
	${CMAKE_SOURCE_DIR}/depends/glfw/include
	${CMAKE_SOURCE_DIR}/depends/assimp/include
)

add_definitions(-DGLM_FORCE_RADIANS)

set(GM_LIBRARY_TYPE STATIC)
if (BUILD_SHARED_LIBS)
	set(GM_LIBRARY_TYPE SHARED)
endif()

add_subdirectory(Core)
add_subdirectory(Framework)
add_subdirectory(Application)

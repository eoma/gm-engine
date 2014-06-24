cmake_minimum_required(VERSION 2.8)

# Framework requirements
include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/depends/glm/include
	${CMAKE_SOURCE_DIR}/depends/ClanLib/Include
)

add_definitions(-DGLM_FORCE_RADIANS)

add_subdirectory(Core)
add_subdirectory(Framework)

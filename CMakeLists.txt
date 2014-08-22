cmake_minimum_required(VERSION 2.8)

project(GmEngine)

option(BUILD_SAMPLES "Build samples" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_SHARED_LIBS "Build the libraries as shared libraries" FALSE)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# Set postfixes for libraries
set(CMAKE_DEBUG_POSTFIX "-debug")

# define an option for choosing between static and dynamic C runtime
set(STATIC_STD_LIBS TRUE CACHE BOOL "TRUE to statically link to the standard libraries, FALSE to use them as DLLs")

#define an option for choosing between uniform and multi-byte character sets
set(UNIFORM_CHARACTER_SET TRUE CACHE BOOL "TRUE to build project with uniform character set, FALSE to build project with multi-byte character set.")

set(EXTRA_COMPILATION_WARNINGS FALSE CACHE BOOL "TRUE to compile all of the code with extra warnings")

configure_file(CompilerSetup.cmake.in
  "${PROJECT_BINARY_DIR}/CompilerSetup.cmake" @ONLY)
include("${PROJECT_BINARY_DIR}/CompilerSetup.cmake") # Compiler setup may be used by both us and external project

###
### Set up dependencies
###

add_subdirectory(depends/gl3w)

# The options are set as internal because the folders containing the code has been removed
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

if(MSVC)
	set(USE_MSVC_RUNTIME_LIBRARY_DLL NOT(${STATIC_STD_LIBS}) CACHE INTERNAL " " FORCE)
endif()

add_subdirectory(depends/glfw)

set_target_properties(glfw
	PROPERTIES
	FOLDER "Dependencies")

add_subdirectory(depends/ClanLib)
add_subdirectory(depends/SOIL2)
add_subdirectory(depends/assimp)
set_target_properties(assimp
	PROPERTIES
	FOLDER "Dependencies")
if (TARGET zlibstatic) # does target zlibstatic exist?
	set_target_properties(zlibstatic
		PROPERTIES
		FOLDER "Dependencies")
endif()

###
### end setup of dependencies
###

add_subdirectory(src)

###
### Set up project configuration
###

set(EXPORT_TARGETS Application Framework Core clanCore SOIL2 assimp glfw gl3w)
if (TARGET zlibstatic)
	list(APPEND EXPORT_TARGETS zlibstatic)
endif()

# Add all relevant targets to the build-tree export set
export(TARGETS ${EXPORT_TARGETS}
  FILE "${PROJECT_BINARY_DIR}/GmEngineTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE GmEngine)

set(CONF_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}/include/
  ${PROJECT_SOURCE_DIR}/samples_common/
  ${PROJECT_SOURCE_DIR}/depends/ClanLib/Include/
  ${PROJECT_SOURCE_DIR}/depends/glm/include/
  ${PROJECT_SOURCE_DIR}/depends/gl3w/include/
  ${PROJECT_SOURCE_DIR}/depends/glfw/include/
  ${PROJECT_SOURCE_DIR}/depends/assimp/include/
)
set(CONF_SOURCE_DIR ${PROJECT_SOURCE_DIR})

set(CONF_DEFINITIONS
	-DGLM_FORCE_RADIANS)

configure_file(GmEngineConfig.cmake.in
  "${PROJECT_BINARY_DIR}/GmEngineConfig.cmake" @ONLY)

###
### End setup project config
###

if (BUILD_SAMPLES)
	add_subdirectory(samples)
endif()

if (BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

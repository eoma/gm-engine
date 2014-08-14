cmake_minimum_required(VERSION 2.8)

project(GmEngine)

option(BUILD_SAMPLES "Build samples" ON)
option(BUILD_TESTS "Build tests" ON)

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
if (TARGET zlibstatic)
	set_target_properties(zlibstatic
		PROPERTIES
		FOLDER "Dependencies")
endif()

###
### end setup of dependencies
###

add_subdirectory(src)


if (BUILD_SAMPLES)
	add_subdirectory(samples)
endif()

if (BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

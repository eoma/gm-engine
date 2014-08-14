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

# Compiler specific properties
if(MSVC)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  if(STATIC_STD_LIBS)
    foreach(flag
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
      CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
      if(${flag} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
      endif()
      if(${flag} MATCHES "/MDd")
        string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
      endif()
    endforeach()
  endif()

  if(UNIFORM_CHARACTER_SET)
    add_definitions(-DUNICODE -D_UNICODE)
  endif()

  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

  if (EXTRA_COMPILATION_WARNINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -std=c++11")
endif()

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

add_subdirectory(src)


if (BUILD_SAMPLES)
	add_subdirectory(samples)
endif()

if (BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

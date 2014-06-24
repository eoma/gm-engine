cmake_minimum_required(VERSION 2.8)

project(gm-engine)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# Set postfixes for libraries
set(CMAKE_DEBUG_POSTFIX "-debug")

# define an option for choosing between static and dynamic C runtime
set(STATIC_STD_LIBS TRUE CACHE BOOL "TRUE to statically link to the standard libraries, FALSE to use them as DLLs")

#define an option for choosing between uniform and multi-byte character sets
set(UNIFORM_CHARACTER_SET TRUE CACHE BOOL "TRUE to build project with uniform character set, FALSE to build project with multi-byte character set.")

# Compiler specific properties
if(MSVC)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  if(STATIC_STD_LIBS)
    foreach(flag
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      if(${flag} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
endif()

add_subdirectory(depends/ClanLib)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

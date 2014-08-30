CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(NAME SOIL2)
PROJECT(${NAME})

option(BUILD_SHARED_LIBS "Build library as shared" OFF)

set(extra_dependent_link_libs)
if (BUILD_SHARED_LIBS)
	if (APPLE)
		find_package(OpenGL REQUIRED)
		find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
		list(APPEND extra_dependent_link_libs
			${OPENGL_gl_LIBRARY}
			${CORE_FOUNDATION_FRAMEWORK})
	endif()
endif()

file(GLOB_RECURSE Headers ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB_RECURSE Sources ${CMAKE_CURRENT_LIST_DIR}/src/*.c)

if (MSVC)
	# Compile SOIL2 as CXX to ensure that required C99 requirements are met
	set_source_files_properties(${Sources}
		PROPERTIES LANGUAGE CXX)
endif()

source_group("Header Files" FILES ${Headers})
source_group("Source Files" FILES ${Sources})

if (BUILD_SHARED_LIBS)
	add_library(${NAME} SHARED ${Headers} ${Sources})
else()
	add_library(${NAME} STATIC ${Headers} ${Sources})
endif()

if (extra_dependent_link_libs)
	target_link_libraries(${NAME} ${extra_dependent_link_libs})
endif()

set_target_properties(${NAME}
	PROPERTIES
	FOLDER "Dependencies")

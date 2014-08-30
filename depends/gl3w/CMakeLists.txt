CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(NAME gl3w)
PROJECT(${NAME})

option(BUILD_SHARED_LIBS "Build library as shared" OFF)

set(extra_dependent_link_libs)
if (BUILD_SHARED_LIBS)
	if (APPLE)
		find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
		list(APPEND extra_dependent_link_libs
			${CORE_FOUNDATION_FRAMEWORK})
	endif()
endif()

file(GLOB_RECURSE Headers ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
file(GLOB_RECURSE Sources ${CMAKE_CURRENT_LIST_DIR}/src/*.c)

source_group("Header Files" FILES ${Headers})
source_group("Source Files" FILES ${Sources})

include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)

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

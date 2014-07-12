CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(NAME SOIL2)
PROJECT(${NAME})

file(GLOB_RECURSE Headers ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB_RECURSE Sources ${CMAKE_CURRENT_LIST_DIR}/src/*.c)

if (MSVC)
	# Compile SOIL2 as CXX to ensure that required C99 requirements are met
	set_source_files_properties(${Sources}
		PROPERTIES LANGUAGE CXX)
endif()

source_group("Header Files" FILES ${Headers})
source_group("Source Files" FILES ${Sources})

add_library(${NAME} STATIC ${Headers} ${Sources})

set_target_properties(${NAME}
	PROPERTIES
	FOLDER "Dependencies")

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(NAME SOIL2)
PROJECT(${NAME})

file(GLOB_RECURSE Headers ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB_RECURSE Sources ${CMAKE_CURRENT_LIST_DIR}/src/*.c)

if (MSVC)
	# Compile SOIL2 as CXX to ensure that required C99 requirements are met
	SET_SOURCE_FILES_PROPERTIES(${Sources}
		PROPERTIES LANGUAGE CXX)
endif()

add_library(${NAME} STATIC ${Headers} ${Sources})

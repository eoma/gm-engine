CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(NAME gl3w)
PROJECT(${NAME})

file(GLOB_RECURSE Headers ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
file(GLOB_RECURSE Sources ${CMAKE_CURRENT_LIST_DIR}/src/*.c)

source_group("Header Files" FILES ${Headers})
source_group("Source Files" FILES ${Sources})

include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)

add_library(${NAME} STATIC ${Headers} ${Sources})

set_target_properties(${NAME}
	PROPERTIES
	FOLDER "Dependencies")

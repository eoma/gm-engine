cmake_minimum_required(VERSION 2.8)

project(LodePNG)

option(BUILD_SHARED_LIBS "Build shared libs" OFF)

set(HEADERS include/lodepng.h)
set(SOURCES src/lodepng.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include)

if (BUILD_SHARED_LIBS)
	add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
else()
	add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
endif()

set_target_properties(${PROJECT_NAME}
	PROPERTIES
	FOLDER "Dependencies")

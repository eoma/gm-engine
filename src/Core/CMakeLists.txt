cmake_minimum_required(VERSION 2.8)

set(NAME Core)
project(${NAME})

file(GLOB_RECURSE Headers ${CMAKE_SOURCE_DIR}/include/GM/Framework *.h)
file(GLOB_RECURSE Sources ${CMAKE_SOURCE_DIR}/src/Framework *.cpp)

#add_library(${NAME} STATIC ${Headers} ${Sources})

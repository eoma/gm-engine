cmake_minimum_required(VERSION 2.8)

set(NAME Framework)
project(${NAME})

file(GLOB_RECURSE Headers ${CMAKE_SOURCE_DIR}/include/GM/Framework *.h)
file(GLOB_RECURSE Sources ${CMAKE_SOURCE_DIR}/src/Framework *.cpp)

add_library(${NAME} STATIC ${Headers} ${Sources})

set_target_properties(${NAME} PROPERTIES FOLDER "GM")
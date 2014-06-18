cmake_minimum_required(VERSION 2.8)

set(NAME Framework)
project(${NAME})

file(GLOB_RECURSE Headers ${CMAKE_CURRENT_LIST_DIR} *.h)
file(GLOB_RECURSE Sources ${CMAKE_CURRENT_LIST_DIR} *.cpp)

add_library(${NAME} STATIC ${Headers} ${Sources})

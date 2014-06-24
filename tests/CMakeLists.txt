cmake_minimum_required(VERSION 2.8)

set(NAME tests)
project(${NAME})

add_definitions(-DGLM_FORCE_RADIANS)

add_subdirectory(Sample)

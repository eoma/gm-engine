cmake_minimum_required(VERSION 2.8)

set(NAME Core)
project(${NAME})

file(GLOB_RECURSE Headers ${CMAKE_SOURCE_DIR}/include/GM/Core *.h)
file(GLOB_RECURSE Sources ${CMAKE_SOURCE_DIR}/src/Core *.cpp)

#source_group("Header Files\\GM\\${NAME}" FILES ${Headers})
#source_group("Source Files\\GM\\${NAME}" FILES ${Sources})

#add_library(${NAME} STATIC ${Headers} ${Sources})

#set_target_properties(${NAME} PROPERTIES FOLDER "GM")

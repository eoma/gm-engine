cmake_minimum_required(VERSION 2.8)

set(NAME tests)
project(${NAME})

add_definitions(-DGLM_FORCE_RADIANS)

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        SET(dirlist ${dirlist} ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(subdirs ${CMAKE_CURRENT_LIST_DIR})

foreach (subdir ${subdirs})
	add_subdirectory(${subdir})
endforeach()

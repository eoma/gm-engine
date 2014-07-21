CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(NAME gl3w)
PROJECT(${NAME})

file(GLOB_RECURSE Headers ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
file(GLOB_RECURSE Sources ${CMAKE_CURRENT_LIST_DIR}/src/*.c)

source_group("Header Files" FILES ${Headers})
source_group("Source Files" FILES ${Sources})

if (UNIX)
    add_definitions(-Wall)

    if (BUILD_SHARED_LIBS)
        add_definitions(-fvisibility=hidden)
    endif()
endif()

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    
	foreach (flag CMAKE_C_FLAGS
					CMAKE_C_FLAGS_DEBUG
					CMAKE_C_FLAGS_RELEASE
					CMAKE_C_FLAGS_MINSIZEREL
					CMAKE_C_FLAGS_RELWITHDEBINFO)

		if (${flag} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
		endif()
		if (${flag} MATCHES "/MDd")
			string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
		endif()

	endforeach()
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)

add_library(${NAME} STATIC ${Headers} ${Sources})

set_target_properties(${NAME}
	PROPERTIES
	FOLDER "Dependencies")

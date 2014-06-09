# TOTEM_EDK_FOUND
# TOTEM_EDK_INCLUDE_PATH
# TOTEM_EDK_HEADERS

find_path(TOTEM_EDK_HOME_PATH
    NAMES include/Totem/Totem.h
    HINTS ENV TOTEM_EDK_HOME # prefer custom checkout
    PATHS ${CMAKE_SOURCE_DIR}/depends/totem-edk # prefer bundled
    DOC "The root directory of TotemEDK")

set(TOTEM_EDK_INCLUDE_PATH ${TOTEM_EDK_HOME_PATH}/include)

file(GLOB_RECURSE TOTEM_EDK_HEADERS
    ${TOTEM_EDK_HOME_PATH}/include/Totem/*.h
    ${TOTEM_EDK_HOME_PATH}/include/Totem/*.inl
	${TOTEM_EDK_HOME_PATH}/include/Totem/Extensions/*.h
    ${TOTEM_EDK_HOME_PATH}/include/Totem/Extensions/*.inl
    ${TOTEM_EDK_HOME_PATH}/depends/Standard/sigslot.h)

#foreach(h ${TOTEM_EDK_HEADERS})
#    message(${h})
#endforeach()

if(TOTEM_EDK_INCLUDE_PATH AND TOTEM_EDK_HEADERS)
    set(TOTEM_FOUND 1 CACHE STRING "Set to 1 if Totem EDK is found, 0 otherwise")
else()
    set(TOTEM_FOUND 0 CACHE STRING "Set to 1 if Totem EDK is found, 0 otherwise")
    message(WARNING "Note: an envvar like TOTEM_EDK_HOME assists this script to locate TOTEM.")
endif()

mark_as_advanced( TOTEM_FOUND )

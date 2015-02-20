CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(NAME ClanLib)
PROJECT(${NAME})

# To build more modules, just add more modules. Naming must be exactly as in source folder.
# Notify that when linking, the naming goes clanCore, clanNetwork, etc.
# Also note that when linking, clanCore should always come last.
set(CLANLIB_MODULES Core)

option(BUILD_SHARED_LIBS "Build shared libs" OFF)

set(CLANLIB_HOME_PATH ${CMAKE_SOURCE_DIR}/depends/ClanLib)
set(CLANLIB_SOURCE_PATH ${CLANLIB_HOME_PATH}/Sources)

include_directories(${CMAKE_SOURCE_DIR}/depends/ClanLib/Sources/API)
include_directories(${CMAKE_SOURCE_DIR}/depends/ClanLib/Sources/)

find_package(Threads REQUIRED)

set(extra_dependent_link_libs ${CMAKE_THREAD_LIBS_INIT})
if (BUILD_SHARED_LIBS)
	if (APPLE)
		find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
		list(APPEND extra_dependent_link_libs ${CORE_FOUNDATION_FRAMEWORK})
	endif()
endif()

###########################################
##	Build the modules

set(CLANLIB_SUFFIX_PART "")
if (MSVC)
	set(CLANLIB_SUFFIX_PART "-static-mt")
	if (BUILD_SHARED_LIBS)
		set(CLANLIB_SUFFIX_PART "-dll")
	endif()
endif()

set(_platformPatterns "win32" "unix")
if (UNIX)
	set(_platformToKeep unix)
else()
	set(_platformToKeep win32)
endif()

file(GLOB_RECURSE CLANLIB_API_HEADERS ${CLANLIB_SOURCE_PATH}/API/*.h)

# Uncomment these and other similar to enable building of a mega library.
# As a consequence it doubles the build time because it needs to rebuild the files.
# You can uncomment the building of the smaller modules to cut down compile time.
#set(CLANLIB_ALLMODS_SOURCES "")
#set(CLANLIB_ALLMODS_HEADERS "")

foreach(module ${CLANLIB_MODULES})
	# Prepare each module for building
	# It will first lookup headers and sources, then remove
	# the headers and source files not relevant for the current platform

	STRING(TOUPPER ${module} MODULE)

	file(GLOB_RECURSE CLANLIB_${MODULE}_HEADERS ${CLANLIB_SOURCE_PATH}/${module}/*.h)
	file(GLOB_RECURSE CLANLIB_${MODULE}_SOURCES ${CLANLIB_SOURCE_PATH}/${module}/*.cpp)

	set(filesToBeRemoved "")

	foreach(fileName ${CLANLIB_${MODULE}_SOURCES})
		string(TOLOWER ${fileName} fileName_lower)

		foreach(platform ${_platformPatterns})
			if (NOT(${platform} STREQUAL ${_platformToKeep}))
				if (${fileName_lower} MATCHES ".*/${platform}/.*")
					#message("${fileName} matches ${platform}")
					list(APPEND filesToBeRemoved ${fileName})
				elseif(${fileName_lower} MATCHES ".*/([^/]*)${platform}([^/]*)$")
					#message("${fileName} matches ${platform}")
					list(APPEND filesToBeRemoved ${fileName})
				endif()
			endif()
		endforeach()
	endforeach()

	foreach(fileToRemove ${filesToBeRemoved})
		list(REMOVE_ITEM CLANLIB_${MODULE}_SOURCES ${fileToRemove})
	endforeach()

	source_group("Header Files" FILES ${CLANLIB_${MODULE}_HEADERS})
	source_group("Source Files" FILES ${CLANLIB_${MODULE}_SOURCES})

	set(headers
		${CLANLIB_API_HEADERS}
		${CLANLIB_${MODULE}_HEADERS}
	)
	set(sources
		${CLANLIB_${MODULE}_SOURCES}
	)

	if (BUILD_SHARED_LIBS)
		add_library(clan${module} SHARED ${headers} ${sources})
	else()
		add_library(clan${module} STATIC ${headers} ${sources})
	endif()

	if (extra_dependent_link_libs)
		target_link_libraries(clan${module} ${extra_dependent_link_libs})
	endif()

	set_target_properties(clan${module} 
		PROPERTIES
		FOLDER "Dependencies"
		RELEASE_POSTFIX "${CLANLIB_SUFFIX_PART}"
		RELWITHDEBINFO_POSTFIX "${CLANLIB_SUFFIX_PART}"
		MINSIZEREL_POSTFIX "${CLANLIB_SUFFIX_PART}"
		DEBUG_POSTFIX "${CLANLIB_SUFFIX_PART}-debug")

	# Uncomment these and other similar to enable building of a mega library
	#list(APPEND CLANLIB_ALLMODS_HEADERS ${CLANLIB_${MODULE}_HEADERS})
	#list(APPEND CLANLIB_ALLMODS_SOURCES ${CLANLIB_${MODULE}_SOURCES})

endforeach()

# Uncomment these and other similar to enable building of a mega library
#add_library(clanAllMods STATIC ${CLANLIB_ALLMODS_HEADERS} ${CLANLIB_ALLMODS_SOURCES})
#target_link_libraries(clanAllMods STATIC ${extra_dependent_link_libs})

###########################################




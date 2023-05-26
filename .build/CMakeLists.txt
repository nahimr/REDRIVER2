cmake_minimum_required(VERSION 3.6)

project(REDRIVER2 C CXX)

set(REDRIVER2_PATH ../src_rebuild)
set(GAME_REGION "NTSC_VERSION")

set(CMAKE_CXX_STANDARD 11)

# Building from sources
if(ANDROID OR EMSCRIPTEN OR IOS OR TVOS)
    if(ANDROID)
        set(__ANDROID__ ON)
    endif()

    # Dependencies Sources
    set(OPENAL_LIBRARY dependencies/openal)
    set(SDL2_LIBRARY dependencies/SDL2)

	if(NOT(OPENAL_LIBRARY))
		message(FATAL_ERROR "OPENAL_LIBRARY is missing !")
	endif()

	if(NOT(SDL2_LIBRARY))
		message(FATAL_ERROR "SDL2_LIBRARY is missing !")
	endif()

	# Dependencies Includes
	set(OPENAL_INCLUDE_DIR ${OPENAL_LIBRARY}/include)
	set(SDL2_INCLUDE_DIR ${SDL2_LIBRARY}/include)

	# Build SDL2 from source
	add_subdirectory(${SDL2_LIBRARY})
	include_directories(${SDL2_INCLUDE_DIR})

	# Build OpenAL from source
	add_subdirectory(${OPENAL_LIBRARY})
	include_directories(${OPENAL_INCLUDE_DIR})
else()
	find_package(SDL2 REQUIRED)
	find_package(OpenAL REQUIRED)
	if(NOT(OPENAL_FOUND))
		message(FATAL_ERROR "Missing OpenAL !")
	endif()

	if(NOT(SDL2_FOUND))
		message(FATAL_ERROR "Missing SDL2 !")
	endif()
endif()

add_subdirectory(dependencies/PsyCross)
add_subdirectory(dependencies/jpeg)

#### REDRIVER 2 PROJECT ####

file(GLOB_RECURSE REDRIVER2_SOURCES ${REDRIVER2_PATH}/*.c ${REDRIVER2_PATH}/*.cpp)
set_source_files_properties(${REDRIVER2_SOURCES} PROPERTIES LANGUAGE CXX)

if(ANDROID OR IOS OR TVOS)
    add_library(REDRIVER2 ${REDRIVER2_SOURCES})
else()
    add_executable(REDRIVER2 ${REDRIVER2_SOURCES})
endif()

add_definitions(
    -DBUILD_CONFIGURATION_STRING="${CMAKE_BUILD_TYPE}_${CMAKE_BUILD_TOOL}"
    -DNTSC_VERSION=1
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(
        -D_DEBUG=1
        -DDEBUG_OPTIONS=1
        -DCOLLISION_DEBUG=1
        -DCUTSCENE_RECORDER=1
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_definitions(
        -DNDEBUG=1
        -DDEBUG_OPTIONS=1
        -DCOLLISION_DEBUG=1
        -DCUTSCENE_RECORDER=1
    )
endif()

if (UNIX AND NOT(CMAKE_CROSSCOMPILING))
    set(PLATFORM_LD_LIBS GL openal)
    target_compile_options(REDRIVER2 PRIVATE
        -Wno-narrowing
        -Wno-endif-labels
        -Wno-write-strings
        -Wno-format-security
        -Wno-unused-result
        -fpermissive
    )
elseif(WIN32)
    target_compile_options(REDRIVER2 PRIVATE
        -Wno-elaborated-enum-class 
        -Wno-narrowing
        -Wno-endif-labels
        -Wno-write-strings
        -Wno-format
        -Wno-format-security
        -Wno-unused-result
        -fpermissive
    )
elseif(ANDROID)
    set(PLATFORM_LD_LIBS GLESv3 EGL OpenSLES log android OpenAL)
    target_compile_definitions(REDRIVER2 PRIVATE -D__ANDROID__=1)
    target_compile_options(REDRIVER2 PRIVATE
        -Wno-narrowing
        -Wno-endif-labels
        -Wno-write-strings
        -Wno-format-security
        -Wno-unused-result
        -fpermissive
    )
endif()

target_include_directories(REDRIVER2 PUBLIC 
    ${REDRIVER2_PATH}/Game 
    ${SDL2_INCLUDE_DIRS}
    ${PsyCross_SOURCE_DIR}/include 
    ${PsyCross_SOURCE_DIR}/include/psx
    ${libjpeg_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/dependencies/jpeg
)

target_link_libraries(REDRIVER2 m PsyCross dl ${PLATFORM_LD_LIBS} jpeg SDL2)

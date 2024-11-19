cmake_minimum_required(VERSION 3.6)

project(REDRIVER2 C CXX)

option(GAME_REGION "Select region (NTSC_VERSION or PAL_VERSION). Default: NTSC_VERSION" "NTSC_VERSION")

# if(CMAKE_SIZEOF_VOID_P EQUAL 8)
#     message(FATAL_ERROR "Project can only be built in 32 bits mode !")
# endif()

set(REDRIVER2_PATH ../src)
set(ALSOFT_EXAMPLES OFF)
set(ALSOFT_UTILS OFF)
set(SDL_TESTS OFF)
set(SDL_TEST OFF)
set(SDL_STATIC OFF)
set(SDL2_DISABLE_SDL2MAIN ON)
set(EXTRA_SOURCES)

if (MSVC)
    set(WIN32 ON CACHE BOOL "Target is Windows 32 Platform")
endif()

if(NOT(CMAKE_BUILD_TYPE))
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Building from sources
if(ANDROID OR EMSCRIPTEN OR IOS OR TVOS OR WIN32)
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

    # set(SDL_TESTS OFF CACHE BOOL "Build SDL Tests")
    # set(SDL_TEST OFF CACHE BOOL "Build SDL Test")
    # set(SDL_STATIC OFF CACHE BOOL "Don't build static SDL")

	add_subdirectory(${SDL2_LIBRARY} EXCLUDE_FROM_ALL)
	include_directories(${SDL2_INCLUDE_DIR})

	# Build OpenAL from source

    # set(ALSOFT_EXAMPLES OFF CACHE "Don't build OpenAL examples" FORCE)
    # set(ALSOFT_UTILS OFF CACHE "Don't build OpenAL utils" FORCE)

	add_subdirectory(${OPENAL_LIBRARY} EXCLUDE_FROM_ALL)
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

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

add_subdirectory(dependencies/PsyCross)

if (NOT(ANDROID))
    set(BUILD_STATIC ON CACHE BOOL "Build JPEG as static")
endif()
# set(BUILD_TESTS OFF CACHE BOOL "Don't build JPEG tests" FORCE)
add_subdirectory(dependencies/jpeg EXCLUDE_FROM_ALL)

#### REDRIVER 2 PROJECT ####

file(GLOB_RECURSE REDRIVER2_SOURCES ${REDRIVER2_PATH}/*.c ${REDRIVER2_PATH}/*.cpp)
file(GLOB_RECURSE REDRIVER2_HEADERS ${REDRIVER2_PATH}/*.h ${REDRIVER2_PATH}/*.hpp)
set_source_files_properties(${REDRIVER2_SOURCES} PROPERTIES LANGUAGE CXX)

## Add resource file ##
if(WIN32)
    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/platforms/windows/Resource.rc LANGUAGE RC)
    list(APPEND EXTRA_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/platforms/windows/Resource.rc)
    source_group(Resources ${EXTRA_SOURCES})
endif()

if(ANDROID OR IOS OR TVOS)
    add_library(REDRIVER2 ${REDRIVER2_SOURCES})
else()
    add_executable(REDRIVER2 ${REDRIVER2_SOURCES} ${REDRIVER2_HEADERS} ${EXTRA_SOURCES})
endif()

target_compile_definitions(REDRIVER2 PUBLIC
    -DBUILD_CONFIGURATION_STRING="${CMAKE_BUILD_TYPE}"
    -DNTSC_VERSION=1
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(REDRIVER2 PUBLIC
        -D_DEBUG=1
        -DDEBUG_OPTIONS=1
        -DCOLLISION_DEBUG=1
        -DCUTSCENE_RECORDER=1
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    target_compile_definitions(REDRIVER2 PUBLIC
        -DNDEBUG=1
        -DDEBUG_OPTIONS=1
        -DCOLLISION_DEBUG=1
        -DCUTSCENE_RECORDER=1
    )
endif()

set(REDRIVER2_INCLUDES
    ${REDRIVER2_PATH}/Game 
    ${SDL2_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/discord
    ${PsyCross_SOURCE_DIR}/include 
    ${PsyCross_SOURCE_DIR}/include/psx
    ${libjpeg_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/dependencies/jpeg
)

target_include_directories(REDRIVER2 PUBLIC ${REDRIVER2_INCLUDES})

### COMPILATION ###

if (UNIX AND NOT(CMAKE_CROSSCOMPILING))
    set(PLATFORM_LD_LIBS GL openal)
    set(PLATFORM_STATIC_LIBS m PsyCross dl)
    target_compile_options(REDRIVER2 PRIVATE
        -Wno-narrowing
        -Wno-endif-labels
        -Wno-write-strings
        -Wno-format-security
        -Wno-unused-result
        -fpermissive
    )
elseif(WIN32)
    set(PLATFORM_LD_LIBS opengl32)
    set(PLATFORM_STATIC_LIBS PsyCross)
    target_compile_options(REDRIVER2 PRIVATE
        /wd4996 
        /wd4554
        /wd4244
        /wd4101
        /wd4838
        /wd4309
    )
elseif(ANDROID)
    set(PLATFORM_LD_LIBS GLESv3 EGL OpenSLES log android OpenAL)
    set(PLATFORM_STATIC_LIBS m PsyCross dl)
    target_compile_definitions(REDRIVER2 PRIVATE -D__ANDROID__=1)
    target_compile_options(REDRIVER2 PRIVATE
        -Wno-narrowing
        -Wno-endif-labels
        -Wno-write-strings
        -Wno-format-security
        -Wno-unused-result
        -fsigned-char
        -fpermissive
    )
endif()

### MSVC ###
if (MSVC)
    # set_property(TARGET REDRIVER2 PROPERTY VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE "x86")
    # set_property(TARGET SDL2 PROPERTY VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE "x86")
    # set_property(TARGET OpenAL PROPERTY VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE "x86")
    # set_property(TARGET jpeg PROPERTY VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE "x86")
    # set_property(TARGET common PROPERTY VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE "x86")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT REDRIVER2)
    set_property(TARGET REDRIVER2 PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../data)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/../src FILES ${REDRIVER2_SOURCES} ${REDRIVER2_HEADERS})
endif()

target_link_libraries(REDRIVER2 ${PLATFORM_STATIC_LIBS} ${PLATFORM_LD_LIBS} jpeg SDL2)

### BUILDING PACKAGES ###

if (WIN32 OR UNIX AND NOT(ANDROID))
    install(TARGETS REDRIVER2 RUNTIME DESTINATION .)

    if (WIN32)
        set(CPACK_GENERATOR "ZIP")
        ### COPY DYNAMIC LIBRARIES TO BUILD DIRECTORIES ###
        add_custom_command (TARGET REDRIVER2 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2> $<TARGET_FILE_DIR:REDRIVER2>
        )

        add_custom_command (TARGET REDRIVER2 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:OpenAL> $<TARGET_FILE_DIR:REDRIVER2>
        )

        install(TARGETS SDL2 RUNTIME DESTINATION .)
        install(TARGETS OpenAL RUNTIME DESTINATION .)
        # install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/platforms/windows/libs/x86/discord_game_sdk.dll" DESTINATION .)
    else()
        set(CPACK_GENERATOR "TGZ")
        install(TARGETS jpeg LIBRARY DESTINATION .)
    endif()

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../data/ DESTINATION .)

    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_${CMAKE_HOST_SYSTEM_NAME}_${CMAKE_BUILD_TYPE}")
    set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
    set(CPACK_SOURCE_IGNORE_FILES
      .git/
      build/
      ".*~$"
      .gitignore
    )
    
    include(CPack)
endif()

if (ANDROID)
    install(TARGETS REDRIVER2 LIBRARY DESTINATION .)
endif()
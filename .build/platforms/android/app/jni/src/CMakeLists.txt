cmake_minimum_required(VERSION 3.6)

project(REDRIVER2)

add_subdirectory(PsyCross)
add_subdirectory(jpeg)

find_library(PsyCross PsyCross)
find_library(jpeg jpeg)

add_compile_options(-Wno-elaborated-enum-class -Wno-narrowing -Wno-endif-labels -Wno-write-strings -Wno-format -Wno-format-security -Wno-unused-result -fpermissive)

file(GLOB_RECURSE REDRIVER2_SOURCES_c ${REDRIVER2_SRC_DIR}/*.c)
file(GLOB_RECURSE REDRIVER2_SOURCES_C ${REDRIVER2_SRC_DIR}/*.C)
file(GLOB_RECURSE REDRIVER2_HEADERS_h ${REDRIVER2_SRC_DIR}/*.h)
file(GLOB_RECURSE REDRIVER2_HEADERS_H ${REDRIVER2_SRC_DIR}/*.H)
file(GLOB_RECURSE REDRIVER2_SOURCES_CPP ${REDRIVER2_SRC_DIR}/*.cpp)

set_source_files_properties(${REDRIVER2_SOURCES_c} PROPERTIES LANGUAGE CXX )
set_source_files_properties(${REDRIVER2_SOURCES_h} PROPERTIES LANGUAGE CXX )
set_source_files_properties(${REDRIVER2_SOURCES_C} PROPERTIES LANGUAGE CXX )
set_source_files_properties(${REDRIVER2_SOURCES_H} PROPERTIES LANGUAGE CXX )


add_library(REDRIVER2 SHARED
            ${REDRIVER2_SOURCES_C}
            ${REDRIVER2_SOURCES_c}
            ${REDRIVER2_HEADERS_H}
            ${REDRIVER2_HEADERS_h}
            ${REDRIVER2_SOURCES_CPP}
           )

include_directories(${SDL2_SOURCE_DIR}/src/include ${REDRIVER2_SRC_DIR}/Game/ ${PSYCROSS_SOURCE_DIR}/include/PsyX ${PSYCROSS_SOURCE_DIR}/include/psx)

target_include_directories(PsyCross PUBLIC
            "${PSYCROSS_SOURCE_DIR}/include"
            "${SDL2_SOURCE_DIR}/include"
            "${OPENAL_SOURCE_DIR}/include"
            "${JPEG_SOURCE_DIR}"
           )

target_link_libraries(REDRIVER2 jpeg m dl GLESv3 EGL OpenSLES log android PsyCross "${PROJECT_DIRECTORY}/libs/${CMAKE_ANDROID_ARCH_ABI}/libSDL2.so" "${PROJECT_DIRECTORY}/libs/${CMAKE_ANDROID_ARCH_ABI}/libopenal.so")
#target_link_libraries(REDRIVER2 jpeg m dl GLESv1_CM GLESv2 OpenSLES log android PsyCross SDL2)



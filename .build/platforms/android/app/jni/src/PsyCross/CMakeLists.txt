cmake_minimum_required(VERSION 3.6)

project(PsyCross C CXX)

add_compile_options(-Wno-narrowing -Wno-endif-labels -Wno-write-strings -Wno-format -Wno-format-security -Wno-unused-result -fpermissive)

find_library(SDL2 SDL2)

file(GLOB_RECURSE PSYCROSS_SOURCES_CXX ${PSYCROSS_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE PSYCROSS_SOURCES_C ${PSYCROSS_SOURCE_DIR}/src/*.C)
file(GLOB_RECURSE PSYCROSS_SOURCES_c ${PSYCROSS_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE PSYCROSS_SOURCES_h ${PSYCROSS_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE PSYCROSS_SOURCES_H ${PSYCROSS_SOURCE_DIR}/src/*.H)
file(GLOB_RECURSE PSYCROSS_SOURCES_HPP ${PSYCROSS_SOURCE_DIR}/src/*.hpp)

add_library(PsyCross STATIC
            ${PSYCROSS_SOURCES_CXX}
            ${PSYCROSS_SOURCES_C}
            ${PSYCROSS_SOURCES_c}
            ${PSYCROSS_SOURCES_HPP}
            ${PSYCROSS_SOURCES_H}
            ${PSYCROSS_SOURCES_h}
           )

include_directories(${SDL2_SOURCE_DIR}/src/include)

target_include_directories(PsyCross PUBLIC
            "${PSYCROSS_SOURCE_DIR}/include"
            "${SDL2_SOURCE_DIR}/include"
            "${OPENAL_SOURCE_DIR}/include"
           )

#target_link_libraries(PsyCross m dl GLESv1_CM GLESv2 OpenSLES log android SDL2)
target_link_libraries(PsyCross m dl GLESv3 EGL OpenSLES log android "${PROJECT_DIRECTORY}/libs/${CMAKE_ANDROID_ARCH_ABI}/libSDL2.so" "${PROJECT_DIRECTORY}/libs/${CMAKE_ANDROID_ARCH_ABI}/libopenal.so")

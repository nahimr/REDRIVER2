cmake_minimum_required(VERSION 3.6)

project(REDRIVER2 C CXX)

add_subdirectory(dependencies/PsyCross)
add_subdirectory(dependencies/jpeg)

# set(JPEG_LIBRARY "dependencies/jpeg")

# #### Building JPEG ####
# file(GLOB_RECURSE JPEG_SOURCES "${JPEG_LIBRARY}/*.c")
# file(GLOB_RECURSE JPEG_HEADERS "${JPEG_LIBRARY}/*.h")

# set_source_files_properties(${JPEG_SOURCES} PROPERTIES LANGUAGE C)
# include_directories(${JPEG_LIBRARY})
# add_library(jpeg STATIC ${JPEG_SOURCES} ${JPEG_HEADERS})

cmake_minimum_required(VERSION 3.6)

project(REDRIVER2)

# armeabi-v7a requires cpufeatures library

include(AndroidNdkModules)
android_ndk_import_module_cpufeatures()

add_definitions(-D__ANDROID__=1 -DUSE_PGXP=0 -DBUILD_CONFIGURATION_STRING="Android Configuration" -DNTSC_VERSION)
set(CMAKE_CXX_STANDARD 11)

# SDL sources are in a subfolder named "SDL"
# add_subdirectory(SDL)

# Compilation of companion libraries
#add_subdirectory(SDL_image)
#add_subdirectory(SDL_mixer)
#add_subdirectory(SDL_ttf)

set(ANDROID TRUE)
set(_ANDROID_ TRUE)
set(__ANDROID__ TRUE)
set(BUILD_ANDROID true)
set(PROJECT_DIRECTORY C:/perso/REDRIVER2/.build/platforms/android)
set(PSYCROSS_SOURCE_DIR C:/perso/REDRIVER2/.build/packages/PsyCross)
set(SDL2_SOURCE_DIR C:/perso/REDRIVER2/.build/packages/SDL2)
set(OPENAL_SOURCE_DIR C:/perso/REDRIVER2/.build/packages/openal-soft)
set(OpenAL_SOURCE_DIR C:/perso/REDRIVER2/.build/packages/openal-soft)
set(JPEG_SOURCE_DIR C:/perso/REDRIVER2/.build/packages/jpeg)
set(REDRIVER2_SRC_DIR C:/perso/REDRIVER2/src)

# Your game and its CMakeLists.txt are in a subfolder named "src"
add_subdirectory(src)


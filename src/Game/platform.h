#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif

#include "psyx_compat.h"

#ifndef PSX
#include "../utils/fs.h"
#endif

#ifdef __GNUC__
#define _stricmp(s1, s2) strcasecmp(s1, s2)
#endif

// Compiler-specific settings
#if defined(_MSC_VER)
    // Microsoft Visual C++ (MSVC) compiler settings
#define ALIGNMENT(x) __declspec(align(x))  // MSVC specific alignment directive
#if defined(WIN32)
typedef int32_t s_int64_t;
#elif defined(WIN64)
typedef long long s_int64_t;
#endif
#elif defined(__GNUC__)
    // GCC or Clang compiler settings
#define ALIGNMENT(x) __attribute__((aligned(x)))  // GCC specific alignment directive
#endif

#endif // PLATFORM_H
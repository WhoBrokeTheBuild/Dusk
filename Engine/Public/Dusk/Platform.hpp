#ifndef DUSK_PLATFORM_HPP
#define DUSK_PLATFORM_HPP

#if defined(_WIN32)

    // Windows
    #define DUSK_OS_WINDOWS

    #ifndef NOMINMAX
        #define NOMINMAX
    #endif

#elif defined(__APPLE__) && defined(__MACH__)

    // Apple
    #include "TargetConditionals.h"

    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        // iPhone iOS
        #define DUSK_OS_IOS
    #elif TARGET_OS_MAC
        // Mac OSX
        #define DUSK_OS_OSX
    #endif

#elif defined(__unix__)

    #if defined(__ANDROID__)
        // Android
        #define DUSK_OS_ANDROID
    #elif defined(__linux__)
        // Linux
        #define DUSK_OS_LINUX
    #else
        #error Unsupported UNIX Operating System
    #endif

#else

    // Unknown
    #error Unsupported Operating System

#endif

#if defined(_MSC_VER)

    // Microsoft VisualStudio C/C++
    #define DUSK_COMPILER_MSVC _MSC_VER

#elif defined(__GNUC__)
    
    // GNU Compiler Collection
    #define DUSK_COMPILER_GCC __GNUC__

#elif defined(__clang__)

    // Apple Clang Compiler
    #define DUSK_COMPILER_CLANG __clang__

#else

    // Unknown
    #warning Unsupported Compiler

#endif

#if defined(DUSK_OS_WINDOWS)

    #define DUSK_PATH_SLASH '\\'
    #define DUSK_PATH_SEPARATOR ';'

#else

    #define DUSK_PATH_SLASH '/'
    #define DUSK_PATH_SEPARATOR ':'

#endif

#endif // DUSK_PLATFORM_HPP
#ifndef DUSK_CONFIG_HPP
#define DUSK_CONFIG_HPP

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

#if DUSK_COMPILER_MSVC
    #define DUSK_API_EXPORT __declspec(dllexport)
    #define DUSK_API_IMPORT __declspec(dllimport)
#else
    #if DUSK_COMPILER_GCC >= 4 || DUSK_COMPILER_CLANG
        #define DUSK_API_EXPORT __attribute__ ((__visibility__ ("default")))
        #define DUSK_API_IMPORT __attribute__ ((__visibility__ ("default")))
    #endif
#endif

#if defined(DUSK_CORE_EXPORT)
    #define DUSK_CORE_API DUSK_API_EXPORT
#else
    #define DUSK_CORE_API DUSK_API_IMPORT
#endif

#endif // DUSK_CONFIG_HPP

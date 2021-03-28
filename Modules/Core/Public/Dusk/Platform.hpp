#ifndef DUSK_PLATFORM_HPP
#define DUSK_PLATFORM_HPP

#if defined(_WIN32)

    // Windows
    #define DUSK_PLATFORM_WINDOWS

    #ifdef __has_include
        #if __has_include(<winapifamily.h>)
            #include <winapifamily.h>
            #if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
                // Universal Windows Platform
                #define DUSK_PLATFORM_UWP
            #endif
        #endif
    #endif

#elif defined(__APPLE__) && defined(__MACH__)

    #define DUSK_PLATFORM_APPLE

    // Apple
    #include "TargetConditionals.h"

    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        // iPhone iOS
        #define DUSK_PLATFORM_IOS
    #elif TARGET_OS_MAC
        // Mac OSX
        #define DUSK_PLATFORM_OSX
    #endif

#elif defined(__unix__)

    #if defined(__ANDROID__)
        // Android
        #define DUSK_PLATFORM_ANDROID
    #elif defined(__linux__)
        // Linux
        #define DUSK_PLATFORM_LINUX
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

#elif defined(__clang__)

    // Apple Clang Compiler
    #define DUSK_COMPILER_CLANG __clang__

#elif defined(__GNUC__) || defined(__GNUG__)
    
    // GNU Compiler Collection
    #define DUSK_COMPILER_GCC __GNUC__

#elif defined(__ICC) || defined(__INTEL_COMPILER)

    #define DUSK_COMPILER_ICC __INTEL_COMPILER

#else

    // Unknown
    #warning Unsupported Compiler

#endif

#if defined(NDEBUG)

    #define DUSK_BUILD_RELEASE

#else

    #define DUSK_BUILD_DEBUG

#endif

#if defined(DUSK_PLATFORM_WINDOWS)

    #define DUSK_PATH_SLASH '\\'
    #define DUSK_PATH_SEPARATOR ';'

    // Disable min/max defines
    #define NOMINMAX

    // Disable python defines for snprintf/vsnprintf
    #define HAVE_SNPRINTF

    // Disable extra includes from Windows.h
    #define VC_EXTRALEAN

    #include <Windows.h>

    #undef FreeModule
    #undef CreateWindow
    
    #undef OPAQUE

#else

    #define DUSK_PATH_SLASH '/'
    #define DUSK_PATH_SEPARATOR ':'

#endif

#endif // DUSK_PLATFORM_HPP
#ifndef DUSK_MACROS_HPP
#define DUSK_MACROS_HPP

///
/// Build Type
///

#if defined(NDEBUG)

    #define DUSK_BUILD_RELEASE

#else

    #define DUSK_BUILD_DEBUG

#endif

///
/// Platform Detection
///

#if defined(_WIN32)

    // Windows
    #define DUSK_PLATFORM_WINDOWS

#elif defined(__linux__)

    // Linux
    #define DUSK_PLATFORM_LINUX

#else

    // Unknown or Unsupported
    #error Unsupported Platform

#endif

///
/// Compiler Detection
///

#if defined(_MSC_VER)

    // Microsoft VisualStudio C/C++
    #define DUSK_COMPILER_MSCV _MSC_VER

#elif defined(__clang__)

    // LLVM Clang Compiler
    #define DUSK_COMPILER_CLANG __clang__

#elif defined(__GNUC__)

    // GNU Compiler Collection
    #define DUSK_COMPILER_GCC __GNUC__

#else

    // Unknown or Unsupported
    #error Unsupported Compiler

#endif

///
/// Asset Path
///

#if !defined(DUSK_ASSET_PATH)

    #define DUSK_ASSET_PATH "./"

#endif

///
/// Function Name
///

#if defined(DUSK_COMPILER_MSCV)

    // e.g. void add(int, int)
    #define DUSK_FUNCTION_NAME __FUNCSIG__

#elif defined(DUSK_COMPILER_CLANG) or defined(DUSK_COMPILER_GCC)

    // e.g. void add(int, int)
    #define DUSK_FUNCTION_NAME __PRETTY_FUNCTION__

#else

    // e.g. add
    #define DUSK_FUNCTION_NAME __func__

#endif

///
/// Stringify
///

#define _DUSK_STRINGIFY(x) #x
#define DUSK_STRINGIFY(x) _DUSK_STRINGIFY(x)

///
/// Linker Import/Export Visibility
///

#if defined(DUSK_COMPILER_MSCV)

    #define DUSK_API_EXPORT _declspec(dllexport)
    #define DUSK_API_IMPORT _declspec(dllimport)

#elif defined(DUSK_COMPILER_CLANG) || defined(DUSK_COMPILER_GCC)

    #define DUSK_API_EXPORT __attribute__((__visibility__("default")))
    #define DUSK_API_IMPORT __attribute__((__visibility__("default")))

#endif

#if defined(DUSK_EXPORT)

    #define DUSK_API DUSK_API_EXPORT

#else

    #define DUSK_API DUSK_API_IMPORT
    
#endif

#endif // DUSK_MACROS_HPP
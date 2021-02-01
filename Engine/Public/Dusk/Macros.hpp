#ifndef DUSK_MACROS_HPP
#define DUSK_MACROS_HPP

#include <Dusk/Platform.hpp>

#define DISALLOW_COPY_AND_ASSIGN(TypeName)              \
    TypeName(const TypeName&) = delete;                 \
    TypeName& operator=(const TypeName&) = delete;

#define DUSK_STRINGIFY(x) _DUSK_STRINGIFY(x)
#define _DUSK_STRINGIFY(x) #x

#if defined(DUSK_COMPILER_MSVC)
    #define DUSK_FUNCTION_NAME() __FUNCSIG__
#elif defined(DUSK_COMPILER_GCC) || defined(DUSK_COMPILER_CLANG)
    #define DUSK_FUNCTION_NAME() __PRETTY_FUNCTION__
#else
    #define DUSK_FUNCTION_NAME() __func__
#endif

#if defined(DUSK_COMPILER_MSVC)
    #define DISABLE_WARNINGS() \
        _Pragma("warning(push, 0)")
    
    #define ENABLE_WARNINGS() \
        _Pragma("warning(pop)")
#elif defined(DUSK_COMPILER_CLANG)
    #define DISABLE_WARNINGS() \
        _Pragma("clang diagnostic push")                    \
        _Pragma("clang diagnostic ignored \"-Wall\"")
    
    #define ENABLE_WARNINGS() \
        _Pragma("clang diagnostic pop")
#elif defined(DUSK_COMPILER_GCC)
    #define DISABLE_WARNINGS() \
        _Pragma("GCC diagnostic push")                      \
        _Pragma("GCC diagnostic ignored \"-Wall\"")
    
    #define ENABLE_WARNINGS() \
        _Pragma("GCC diagnostic pop")
#else
    #define DISABLE_WARNINGS()
#endif

#endif // DUSK_MACROS_HPP

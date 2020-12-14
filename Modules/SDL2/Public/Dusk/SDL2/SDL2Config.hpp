#ifndef DUSK_SDL2_CONFIG_HPP
#define DUSK_SDL2_CONFIG_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_SDL2_EXPORT)
    #define DUSK_SDL2_API DUSK_API_EXPORT
#else
    #define DUSK_SDL2_API DUSK_API_IMPORT
#endif

#include <SDL.h>

#undef CreateWindow
#undef near
#undef far

#endif // DUSK_SDL2_CONFIG_HPP

#ifndef DUSK_OPENGL_CONFIG_HPP
#define DUSK_OPENGL_CONFIG_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_OPENGL_EXPORT)
    #define DUSK_OPENGL_API DUSK_API_EXPORT
#else
    #define DUSK_OPENGL_API DUSK_API_IMPORT
#endif

#define WIN32_LEAN_AND_MEAN

#include <glad/gl.h>

#include <Dusk/SDL2/SDL2Config.hpp>
#include <SDL_opengl.h>

#endif // DUSK_OPENGL_CONFIG_HPP

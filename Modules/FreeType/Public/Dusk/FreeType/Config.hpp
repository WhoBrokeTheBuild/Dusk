#ifndef DUSK_FREETYPE_HPP
#define DUSK_FREETYPE_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_FREETYPE_EXPORT)
    #define DUSK_FREETYPE_API DUSK_API_EXPORT
#else
    #define DUSK_FREETYPE_API DUSK_API_IMPORT
#endif

#include <glad/gl.h>
#undef near
#undef far

#endif // DUSK_FREETYPE_HPP

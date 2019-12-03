#ifndef DUSK_OPENGL_HPP
#define DUSK_OPENGL_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_OPENGL_EXPORT)
    #define DUSK_OPENGL_API DUSK_API_EXPORT
#else
    #define DUSK_OPENGL_API DUSK_API_IMPORT
#endif

#endif // DUSK_OPENGL_HPP

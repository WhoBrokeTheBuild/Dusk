#ifndef DUSK_TINYOBJ_HPP
#define DUSK_TINYOBJ_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_TINYOBJ_EXPORT)
    #define DUSK_TINYOBJ_API DUSK_API_EXPORT
#else
    #define DUSK_TINYOBJ_API DUSK_API_IMPORT
#endif

#endif // DUSK_TINYOBJ_HPP

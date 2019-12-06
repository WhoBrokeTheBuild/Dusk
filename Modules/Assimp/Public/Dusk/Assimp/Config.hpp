#ifndef DUSK_ASSIMP_HPP
#define DUSK_ASSIMP_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_ASSIMP_EXPORT)
    #define DUSK_ASSIMP_API DUSK_API_EXPORT
#else
    #define DUSK_ASSIMP_API DUSK_API_IMPORT
#endif

#endif // DUSK_ASSIMP_HPP

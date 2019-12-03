#ifndef DUSK_GLTF2_HPP
#define DUSK_GLTF2_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_GLTF2_EXPORT)
    #define DUSK_GLTF2_API DUSK_API_EXPORT
#else
    #define DUSK_GLTF2_API DUSK_API_IMPORT
#endif

#endif // DUSK_GLTF2_HPP

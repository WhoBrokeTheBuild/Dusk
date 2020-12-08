#ifndef DUSK_STBI_CONFIG_HPP
#define DUSK_STBI_CONFIG_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_STBI_EXPORT)
    #define DUSK_STBI_API DUSK_API_EXPORT
#else
    #define DUSK_STBI_API DUSK_API_IMPORT
#endif

#endif // DUSK_STBI_CONFIG_HPP

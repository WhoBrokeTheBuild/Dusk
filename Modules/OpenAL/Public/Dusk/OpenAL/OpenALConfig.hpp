#ifndef DUSK_OPENAL_CONFIG_HPP
#define DUSK_OPENAL_CONFIG_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_OPENAL_EXPORT)
    #define DUSK_OPENAL_API DUSK_API_EXPORT
#else
    #define DUSK_OPENAL_API DUSK_API_IMPORT
#endif

#endif // DUSK_OPENAL_CONFIG_HPP

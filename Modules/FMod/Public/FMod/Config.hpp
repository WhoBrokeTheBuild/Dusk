#ifndef DUSK_FMOD_HPP
#define DUSK_FMOD_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_FMOD_EXPORT)
    #define DUSK_FMOD_API DUSK_API_EXPORT
#else
    #define DUSK_FMOD_API DUSK_API_IMPORT
#endif

#endif // DUSK_FMOD_HPP

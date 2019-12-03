#ifndef DUSK_CORE_HPP
#define DUSK_CORE_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_CORE_EXPORT)
    #define DUSK_CORE_API DUSK_API_EXPORT
#else
    #define DUSK_CORE_API DUSK_API_IMPORT
#endif

#endif // DUSK_CORE_HPP

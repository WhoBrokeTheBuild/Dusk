#ifndef DUSK_DIRECTX_HPP
#define DUSK_DIRECTX_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_DIRECTX_EXPORT)
    #define DUSK_DIRECTX_API DUSK_API_EXPORT
#else
    #define DUSK_DIRECTX_API DUSK_API_IMPORT
#endif

#endif // DUSK_DIRECTX_HPP

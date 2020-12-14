#ifndef DUSK_STBI_CONFIG_HPP
#define DUSK_STBI_CONFIG_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_STBI_EXPORT)
    #define DUSK_STBI_API DUSK_API_EXPORT
#else
    #define DUSK_STBI_API DUSK_API_IMPORT
#endif

#define STB_NO_HDR
#define STB_NO_PSD
#define STB_NO_PIC
#define STB_NO_PNM
#include <stb/stb_image.h>

#endif // DUSK_STBI_CONFIG_HPP

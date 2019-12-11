#ifndef DUSK_VULKAN_HPP
#define DUSK_VULKAN_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_VULKAN_EXPORT)
    #define DUSK_VULKAN_API DUSK_API_EXPORT
#else
    #define DUSK_VULKAN_API DUSK_API_IMPORT
#endif

#include <glad/vulkan.h>
#undef near
#undef far

#endif // DUSK_VULKAN_HPP

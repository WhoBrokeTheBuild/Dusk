#ifndef DUSK_VULKAN_CONFIG_HPP
#define DUSK_VULKAN_CONFIG_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_VULKAN_EXPORT)
    #define DUSK_VULKAN_API DUSK_API_EXPORT
#else
    #define DUSK_VULKAN_API DUSK_API_IMPORT
#endif

#define WIN32_LEAN_AND_MEAN

#include <glad/vulkan.h>

#include <SDL.h>
#include <SDL_vulkan.h>

#undef CreateWindow
#undef near
#undef far

#if !defined(NDEBUG)
    #define DUSK_VULKAN_VALIDATION_LAYER
#endif

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#endif // DUSK_VULKAN_CONFIG_HPP

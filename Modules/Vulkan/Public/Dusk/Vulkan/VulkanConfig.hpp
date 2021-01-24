#ifndef DUSK_VULKAN_CONFIG_HPP
#define DUSK_VULKAN_CONFIG_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_VULKAN_EXPORT)
    #define DUSK_VULKAN_API DUSK_API_EXPORT
#else
    #define DUSK_VULKAN_API DUSK_API_IMPORT
#endif

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glad/vulkan.h>

#include <Dusk/SDL2/SDL2Config.hpp>
#include <SDL_vulkan.h>

DISABLE_WARNINGS()

    #include <vk_mem_alloc.h>

ENABLE_WARNINGS()

#endif // DUSK_VULKAN_CONFIG_HPP

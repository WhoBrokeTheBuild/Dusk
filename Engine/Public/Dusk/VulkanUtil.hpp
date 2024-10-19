#ifndef VULKAN_UTIL_HPP
#define VULKAN_UTIL_HPP

#include <Dusk/String.hpp>
#include <Dusk/Exception.hpp>

#include <Dusk/ThirdParty/vulkan.hpp>

namespace dusk {

StringView VkResultToString(VkResult result);

StringView VkObjectTypeToString(VkObjectType objectType);

StringView VkFormatToString(VkFormat format);

StringView VkColorSpaceToString(VkColorSpaceKHR colorSpace);

StringView VkPresentModeToString(VkPresentModeKHR presentMode);

inline void CheckVkResult(StringView function, VkResult result) {
    if (result != VK_SUCCESS) {
        throw Exception("{} returned {}", function, VkResultToString(result));
    }
}

} // namespace dusk

#endif // VULKAN_UTIL_HPP
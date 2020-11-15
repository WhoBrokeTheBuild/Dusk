
SET(_target DuskVulkan)

DEFINE_MODULE(${_target} VULKAN)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        SDL2::SDL2
)
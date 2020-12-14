
SET(_target DuskVulkan)

DEFINE_MODULE(${_target} VULKAN)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        DuskSDL2
)
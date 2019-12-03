
SET(_TARGET DuskVulkan)

Module(${_TARGET} VULKAN)

TARGET_LINK_LIBRARIES(
    ${_TARGET}
    PUBLIC
        DuskCore
)
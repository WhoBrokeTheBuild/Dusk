
SET(_TARGET DuskVulkan)

IF(TARGET SDL2::SDL2)
    Module(${_TARGET} VULKAN)

    TARGET_LINK_LIBRARIES(
        ${_TARGET}
        PUBLIC
            DuskCore
            SDL2::SDL2
    )
ELSE()
    MESSAGE(WARNING "Not building ${_TARGET}, missing SDL2::SDL2")
ENDIF()
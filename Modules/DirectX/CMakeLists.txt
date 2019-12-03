
SET(_TARGET DuskDirectX)

Module(${_TARGET} DIRECTX)

TARGET_LINK_LIBRARIES(
    ${_TARGET}
    PUBLIC
        DuskCore
)
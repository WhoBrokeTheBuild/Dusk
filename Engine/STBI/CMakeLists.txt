
SET(_TARGET DuskSTBI)

Module(${_TARGET} STBI)

TARGET_LINK_LIBRARIES(
    ${_TARGET}
    PUBLIC
        DuskCore
)

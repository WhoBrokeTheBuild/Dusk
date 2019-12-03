
SET(_TARGET DuskAssimp)

Module(${_TARGET} ASSIMP)

TARGET_LINK_LIBRARIES(
    ${_TARGET}
    PUBLIC
        DuskCore
)
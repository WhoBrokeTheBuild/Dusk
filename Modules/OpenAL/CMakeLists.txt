
SET(_TARGET DuskOpenAL)

Module(${_TARGET} OPENAL)

TARGET_LINK_LIBRARIES(
    ${_TARGET}
    PUBLIC
        DuskCore
)
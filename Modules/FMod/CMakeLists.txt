
SET(_TARGET DuskFMod)

Module(${_TARGET} FMOD)

TARGET_LINK_LIBRARIES(
    ${_TARGET}
    PUBLIC
        DuskCore
)
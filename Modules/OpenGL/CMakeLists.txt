
SET(_TARGET DuskOpenGL)

Module(${_TARGET} OPENGL)

TARGET_LINK_LIBRARIES(
    ${_TARGET}
    PUBLIC
        DuskCore
        SDL2::SDL2
)

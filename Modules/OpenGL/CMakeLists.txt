
SET(_target DuskOpenGL)

DEFINE_MODULE(${_target} OPENGL)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        SDL2::SDL2
)


SET(_TARGET DuskOpenGL)

IF(TARGET SDL2::SDL2)
    DUSK_MODULE(${_TARGET} OPENGL)

    TARGET_LINK_LIBRARIES(
        ${_TARGET}
        PUBLIC
            DuskCore
            SDL2::SDL2
    )
ELSE()
    MESSAGE(WARNING "Not building ${_TARGET}, missing SDL2::SDL2")
ENDIF()
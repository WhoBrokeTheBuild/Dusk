
SET(_TARGET DuskOpenAL)

IF(TARGET OpenAL::OpenAL)
    DUSK_MODULE(${_TARGET} OPENAL)

    TARGET_LINK_LIBRARIES(
        ${_TARGET}
        PUBLIC
            DuskCore
            OpenAL::OpenAL
    )
ELSE()
    MESSAGE(WARNING "Not building ${_TARGET}, missing OpenAL::OpenAL")
ENDIF()
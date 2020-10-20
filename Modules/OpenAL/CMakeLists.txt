
SET(_TARGET DuskOpenAL)

IF(TARGET OpenAL::OpenAL)
    DUSK_MODULE(${_TARGET} OPENAL)

    LIST(APPEND MODULE_TARGETS ${_TARGET})
    SET(MODULE_TARGETS ${MODULE_TARGETS} PARENT_SCOPE)

    TARGET_LINK_LIBRARIES(
        ${_TARGET}
        PUBLIC
            DuskEngine
            OpenAL::OpenAL
    )
ELSE()
    MESSAGE(STATUS "Not building ${_TARGET}, missing OpenAL::OpenAL")
ENDIF()
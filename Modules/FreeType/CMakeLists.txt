
SET(_TARGET DuskFreeType)

IF(TARGET freetype)
    DUSK_MODULE(${_TARGET} FREETYPE)

    LIST(APPEND MODULE_TARGETS ${_TARGET})
    SET(MODULE_TARGETS ${MODULE_TARGETS} PARENT_SCOPE)

    TARGET_LINK_LIBRARIES(
        ${_TARGET}
        PUBLIC
            DuskEngine
            Freetype::Freetype
    )
ELSE()
    MESSAGE(WARNING "Not building ${_TARGET}, missing freetype")
ENDIF()
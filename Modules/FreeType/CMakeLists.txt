
SET(_TARGET DuskFreeType)

IF(TARGET freetype)
    DUSK_MODULE(${_TARGET} FREETYPE)

    TARGET_LINK_LIBRARIES(
        ${_TARGET}
        PUBLIC
            DuskCore
            freetype
    )
ELSE()
    MESSAGE(WARNING "Not building ${_TARGET}, missing freetype")
ENDIF()

SET(_TARGET DuskAssimp)

IF(TARGET assimp::assimp)
    DUSK_MODULE(${_TARGET} ASSIMP)

    LIST(APPEND MODULE_TARGETS ${_TARGET})
    SET(MODULE_TARGETS ${MODULE_TARGETS} PARENT_SCOPE)

    TARGET_LINK_LIBRARIES(
        ${_TARGET}
        PUBLIC
            DuskEngine
            assimp::assimp
    )
ELSE()
    MESSAGE(WARNING "Not building ${_TARGET}, missing assimp::assimp")
ENDIF()
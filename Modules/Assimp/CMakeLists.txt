
SET(_TARGET DuskAssimp)

IF(TARGET assimp::assimp)
    Module(${_TARGET} ASSIMP)

    TARGET_LINK_LIBRARIES(
        ${_TARGET}
        PUBLIC
            DuskCore
            assimp::assimp
    )
ELSE()
    MESSAGE(WARNING "Not building ${_TARGET}, missing assimp::assimp")
ENDIF()
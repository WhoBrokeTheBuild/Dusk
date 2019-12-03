
SET(_TARGET DuskAssimp)

IF(TARGET assimp::assimp)
    Module(${_TARGET} ASSIMP)

    # Extract release-specific configuration
    GET_TARGET_PROPERTY(soname   assimp::assimp IMPORTED_SONAME_RELEASE)
    GET_TARGET_PROPERTY(implib   assimp::assimp IMPORTED_IMPLIB_RELEASE)
    GET_TARGET_PROPERTY(location assimp::assimp IMPORTED_LOCATION_RELEASE)

    # Set build type agnostic configuration
    SET_TARGET_PROPERTIES(
        assimp::assimp 
        PROPERTIES
            IMPORTED_SONAME     "${soname}"
            IMPORTED_IMPLIB     "${implib}"
            IMPORTED_LOCATION   "${location}"
    )

    TARGET_LINK_LIBRARIES(
        ${_TARGET}
        PUBLIC
            DuskCore
            assimp::assimp
    )
ELSE()
    MESSAGE(WARNING "Not building Assimp module, missing assimp::assimp")
ENDIF()
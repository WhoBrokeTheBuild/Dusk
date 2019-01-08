CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)

LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/../CMake")

INCLUDE(ExternalProject)

# glm

FIND_PACKAGE(GLM QUIET)

IF(NOT GLM_FOUND)
    SET(_GLM_DIR "${CMAKE_BINARY_DIR}/glm")

    ExternalProject_Add(
        ThirdParty_glm
        GIT_REPOSITORY      "https://github.com/g-truc/glm.git"
        GIT_TAG             "0.9.9.3"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${_GLM_DIR}"
        CONFIGURE_COMMAND   ""
        BUILD_COMMAND       ""
        UPDATE_COMMAND      ""
        INSTALL_COMMAND     ""
        TEST_COMMAND        ""
    )
ENDIF()

# json

FIND_PACKAGE(JSON QUIET)

IF(NOT JSON_FOUND)
    SET(_JSON_DIR "${CMAKE_BINARY_DIR}/json")

    ExternalProject_Add(
        ThirdParty_json
        GIT_REPOSITORY      "https://github.com/nlohmann/json.git"
        GIT_TAG             "master"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${_JSON_DIR}"
        CONFIGURE_COMMAND   ""
        BUILD_COMMAND       ""
        UPDATE_COMMAND      ""
        INSTALL_COMMAND     ""
        TEST_COMMAND        ""
    )
ENDIF()

# SDL2

FIND_PACKAGE(SDL2 QUIET)

IF(NOT SDL2_FOUND)
    SET(_SDL2_DIR "${CMAKE_BINARY_DIR}/SDL2")
    SET(_SDL2_SOURCE_DIR "${CMAKE_BINARY_DIR}/SDL2_src")
    SET(_SDL2_BINARY_DIR "${CMAKE_BINARY_DIR}/SDL2_bin")

    ExternalProject_Add(
        ThirdParty_SDL2
        GIT_REPOSITORY      "https://github.com/spurious/SDL-mirror.git"
        GIT_TAG             "master"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${_SDL2_SOURCE_DIR}"
        BINARY_DIR          "${_SDL2_BINARY_DIR}"
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${_SDL2_DIR}
        BUILD_COMMAND       ""
        INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install --config Release
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )
ENDIF()

# OpenAL

FIND_PACKAGE(OpenAL QUIET)

IF(NOT OpenAL_FOUND)
    SET(_OpenAL_DIR "${CMAKE_BINARY_DIR}/OpenAL")
    SET(_OpenAL_SOURCE_DIR "${CMAKE_BINARY_DIR}/OpenAL_src")
    SET(_OpenAL_BINARY_DIR "${CMAKE_BINARY_DIR}/OpenAL_bin")

    ExternalProject_Add(
        ThirdParty_OpenAL
        GIT_REPOSITORY      "https://github.com/kcat/openal-soft.git"
        GIT_TAG             "v1.19"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${_OpenAL_SOURCE_DIR}"
        BINARY_DIR          "${_OpenAL_BINARY_DIR}"
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${_OpenAL_DIR}
                            -DALSOFT_UTILS=OFF
                            -DALSOFT_EXAMPLES=OFF
                            -DALSOFT_TESTS=OFF
        BUILD_COMMAND       ""
        INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install --config Release
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )
ENDIF()

# Ogg, Vorbis, Theora

FIND_PACKAGE(Ogg QUIET)

IF(NOT Ogg_FOUND)
    SET(_Ogg_DIR "${CMAKE_BINARY_DIR}/Ogg")
    SET(_Ogg_SOURCE_DIR "${CMAKE_BINARY_DIR}/Ogg_src")

    SET(_WITH_OGG --with-ogg=${_Ogg_DIR})

    SET(CONFIGURE_COMMAND "")
    IF(MSVC) 
        SET(CONFIGURE_COMMAND ${CMAKE_VS_MSBUILD_COMMAND} win32/VS2015/libogg_dynamic.vcproj /p:OutputPath=${_Ogg_DIR})
    ELSE()
        SET(CONFIGURE_COMMAND ./autogen.sh && ./configure --prefix=${_Ogg_DIR} && ${CMAKE_MAKE_PROGRAM} install -l )
    ENDIF()

    ExternalProject_Add(
        ThirdParty_Ogg
        GIT_REPOSITORY      "https://github.com/xiph/ogg.git"
        GIT_TAG             "v1.3.3"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${_Ogg_SOURCE_DIR}"
        BUILD_IN_SOURCE     TRUE
        CONFIGURE_COMMAND   ${CONFIGURE_COMMAND}
        BUILD_COMMAND       ""
        INSTALL_COMMAND     ""
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )
ENDIF()

IF(NOT Vorbis_FOUND)
    SET(_Vorbis_DIR "${CMAKE_BINARY_DIR}/Vorbis")
    SET(_Vorbis_SOURCE_DIR "${CMAKE_BINARY_DIR}/Vorbis_src")
    
    SET(_WITH_VORBIS --with-vorbis=${_Vorbis_DIR})

    SET(CONFIGURE_COMMAND "")
    IF(MSVC)
        SET(CONFIGURE_COMMAND ${CMAKE_VS_MSBUILD_COMMAND} win32/VS2010/libvorbis_dynamic.vcproj /p:OutputPath=${_Vorbis_DIR})
    ELSE()
        SET(CONFIGURE_COMMAND ./autogen.sh && ./configure ${_WITH_OGG} --prefix=${_Vorbis_DIR} && ${CMAKE_MAKE_PROGRAM} install -l )
    ENDIF()

    ExternalProject_Add(
        ThirdParty_Vorbis
        GIT_REPOSITORY      "https://github.com/xiph/vorbis.git"
        GIT_TAG             "v1.3.6"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${_Vorbis_SOURCE_DIR}"
        BUILD_IN_SOURCE     TRUE
        CONFIGURE_COMMAND   ${CONFIGURE_COMMAND}
        BUILD_COMMAND       ""
        INSTALL_COMMAND     ""
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )
    
    ADD_DEPENDENCIES(ThirdParty_Vorbis ThirdParty_Ogg)
ENDIF()

IF(NOT Theora_FOUND)
    SET(_Theora_DIR "${CMAKE_BINARY_DIR}/Theora")
    SET(_Theora_SOURCE_DIR "${CMAKE_BINARY_DIR}/Theora_src")

    SET(CONFIGURE_COMMAND "")
    IF(MSVC)
        SET(CONFIGURE_COMMAND ${CMAKE_VS_MSBUILD_COMMAND} win32/VS2010/libtheora/libtheora_dynamic.vcxproj /p:OutputPath=${_Theora_DIR})
    ELSE()
        SET(CONFIGURE_COMMAND ./autogen.sh ${_WITH_OGG} ${_WITH_VORBIS} --prefix=${_Theora_DIR} --disable-examples && ${CMAKE_MAKE_PROGRAM} install -l )
    ENDIF()

    ExternalProject_Add(
        ThirdParty_Theora
        GIT_REPOSITORY      "https://github.com/xiph/theora.git"
        GIT_TAG             "v1.2.0alpha1"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${_Theora_SOURCE_DIR}"
        BUILD_IN_SOURCE     TRUE
        CONFIGURE_COMMAND   ${CONFIGURE_COMMAND}
        BUILD_COMMAND       ""
        INSTALL_COMMAND     ""
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )
    
    ADD_DEPENDENCIES(ThirdParty_Theora ThirdParty_Ogg ThirdParty_Vorbis)
ENDIF()

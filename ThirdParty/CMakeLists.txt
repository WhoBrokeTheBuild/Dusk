CMAKE_MINIMUM_REQUIRED(VERSION 3.12 FATAL_ERROR)

LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/../CMake")

# Include CMAKE_POLICY decisions
INCLUDE(Policies)

PROJECT(ThirdParty)

INCLUDE(ExternalProject)

# glm

FIND_PACKAGE(GLM QUIET)

IF(NOT GLM_FOUND)
    SET(_GLM_DIR "${CMAKE_BINARY_DIR}/glm")
    SET(_GLM_SOURCE_DIR "${CMAKE_BINARY_DIR}/glm_src")

    ExternalProject_Add(
        ThirdParty_glm
        GIT_REPOSITORY      "https://github.com/g-truc/glm.git"
        GIT_TAG             "0.9.9.3"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${_GLM_SOURCE_DIR}"
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${_GLM_DIR}
                            -DGLM_TEST_ENABLE=OFF
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )
ENDIF()

# json

FIND_PACKAGE(JSON QUIET)

IF(NOT JSON_FOUND)
    SET(_JSON_DIR "${CMAKE_BINARY_DIR}/json")
    SET(_JSON_SOURCE_DIR "${CMAKE_BINARY_DIR}/json_src")

    ExternalProject_Add(
        ThirdParty_json
        GIT_REPOSITORY      "https://github.com/nlohmann/json.git"
        GIT_TAG             "master"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${_JSON_SOURCE_DIR}"
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${_JSON_DIR}
                            -DJSON_BuildTests=OFF
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )
ENDIF()

# GTest

IF(BUILD_TESTS)
    FIND_PACKAGE(GTest QUIET)

    IF(NOT GTEST_FOUND)
        SET(_GTest_DIR "${CMAKE_BINARY_DIR}/GTest")
        SET(_GTest_SOURCE_DIR "${CMAKE_BINARY_DIR}/GTest_src")
        SET(_GTest_BINARY_DIR "${CMAKE_BINARY_DIR}/GTest_bin")

        ExternalProject_Add(
            ThirdParty_GTest
            GIT_REPOSITORY      "https://github.com/google/googletest"
            GIT_TAG             "v1.10.x"
            GIT_SHALLOW         TRUE
            SOURCE_DIR          "${_GTest_SOURCE_DIR}"
            BINARY_DIR          "${_GTest_BINARY_DIR}"
            CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${_GTest_DIR}
                                -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            UPDATE_COMMAND      ""
            TEST_COMMAND        ""
        )
    ENDIF()
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
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )
ENDIF()

# Python3

FIND_PACKAGE(Python3 QUIET COMPONENTS Development)

IF(NOT Python3_Development_FOUND)
    SET(_Python3_DIR "${CMAKE_BINARY_DIR}/Python3")
    SET(_Python3_SOURCE_DIR "${CMAKE_BINARY_DIR}/Python3_src")

    SET(_Python3_INSTALL_COMMAND "")
    SET(_Python3_INSTALL_COMMAND1 "")
    SET(_Python3_INSTALL_COMMAND2 "")

    IF(WIN32)
        IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
            SET(ARCH "x64")
        ELSEIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
            SET(ARCH "Win32")
        ENDIF()
        SET(_Python3_INSTALL_COMMAND "")
        SET(_Python3_INSTALL_COMMAND1 cmd /c PCbuild\\build.bat -c Debug -t Build -p ${ARCH})
        SET(_Python3_INSTALL_COMMAND2 cmd /c PCbuild\\build.bat -c Release -t Build -p ${ARCH})
    ELSE()
        SET(_Python3_INSTALL_COMMAND ./configure --prefix=${_Python3_DIR} --enable-shared --enable-optimizations)
        SET(_Python3_INSTALL_COMMAND1 ${CMAKE_MAKE_PROGRAM} -l libpython3.so install)
    ENDIF()

    MESSAGE(STATUS ${CMAKE_INSTALL_PREFIX})

    ExternalProject_Add(
        ThirdParty_Python3
        GIT_REPOSITORY      "https://github.com/python/cpython.git"
        GIT_TAG             "3.8"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${_Python3_SOURCE_DIR}"
        BUILD_IN_SOURCE     TRUE
        CONFIGURE_COMMAND   "${_Python3_INSTALL_COMMAND}"
        BUILD_COMMAND       ""
        UPDATE_COMMAND      ""
        INSTALL_COMMAND     "${_Python3_INSTALL_COMMAND1}"
        COMMAND             "${_Python3_INSTALL_COMMAND2}"
        TEST_COMMAND        ""
    )
ENDIF()

# Assimp

SET(_assimp_DIR "${CMAKE_BINARY_DIR}/assimp")

MESSAGE(STATUS "${CMAKE_MODULE_PATH}")

FIND_PACKAGE(assimp QUIET PATHS "${_assimp_DIR}/lib/cmake/assimp-5.0")

IF(NOT assimp_FOUND)
    SET(_assimp_SOURCE_DIR "${CMAKE_BINARY_DIR}/assimp_src")
    SET(_assimp_BINARY_DIR "${CMAKE_BINARY_DIR}/assimp_bin")

    ExternalProject_Add(
        ThirdParty_assimp
        GIT_REPOSITORY      "https://github.com/assimp/assimp.git"
        GIT_TAG             "master"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${_assimp_SOURCE_DIR}"
        BINARY_DIR          "${_assimp_BINARY_DIR}"
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${_assimp_DIR}
                            -DASSIMP_BUILD_ASSIMP_TOOLS=OFF
                            -DASSIMP_BUILD_SAMPLES=OFF
                            -DASSIMP_BUILD_TESTS=OFF
                            -DBUILD_DOCS=OFF
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
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )
ENDIF()

# Ogg, Vorbis, Theora

FIND_PACKAGE(Ogg QUIET COMPONENTS Vorbis Theora)

IF(NOT Ogg_FOUND)
    SET(_Ogg_DIR "${CMAKE_BINARY_DIR}/libogg")
    SET(_Ogg_SOURCE_DIR "${CMAKE_BINARY_DIR}/libogg_src")

    SET(_WITH_OGG --with-ogg=${_Ogg_DIR})

    SET(_Ogg_INSTALL_COMMAND1 "")
    SET(_Ogg_INSTALL_COMMAND2 "")
    SET(_Ogg_INSTALL_COMMAND3 "")
    SET(_Ogg_INSTALL_COMMAND4 "")
    IF(MSVC) 
        SET(SOLUTION win32/VS2015/libogg_static.sln)
        SET(_Ogg_INSTALL_COMMAND1 ${CMAKE_VS_DEVENV_COMMAND} ${SOLUTION} /Upgrade )
        SET(_Ogg_INSTALL_COMMAND2 ${CMAKE_VS_MSBUILD_COMMAND} ${SOLUTION} /t:libogg_static /clp:ErrorsOnly /p:OutDir=${_Ogg_DIR}/lib/ )
        SET(_Ogg_INSTALL_COMMAND3 ${CMAKE_COMMAND} -E make_directory ${_Ogg_DIR}/include/ogg)
        SET(_Ogg_INSTALL_COMMAND4 ${CMAKE_COMMAND} -E copy include/ogg/ogg.h include/ogg/os_types.h ${_Ogg_DIR}/include/ogg)
    ELSE()
        SET(_Ogg_INSTALL_COMMAND1 ./autogen.sh )
        SET(_Ogg_INSTALL_COMMAND2 ./configure --prefix=${_Ogg_DIR} )
        SET(_Ogg_INSTALL_COMMAND3 ${CMAKE_MAKE_PROGRAM} install -l )
    ENDIF()

    ExternalProject_Add(
        ThirdParty_Ogg
        GIT_REPOSITORY      "https://github.com/xiph/ogg.git"
        GIT_TAG             "v1.3.3"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${_Ogg_SOURCE_DIR}"
        BUILD_IN_SOURCE     TRUE
        CONFIGURE_COMMAND   ""
        BUILD_COMMAND       ""
        INSTALL_COMMAND     ${_Ogg_INSTALL_COMMAND1}
        COMMAND             ${_Ogg_INSTALL_COMMAND2}
        COMMAND             ${_Ogg_INSTALL_COMMAND3}
        COMMAND             ${_Ogg_INSTALL_COMMAND4}
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )

    SET(_Vorbis_DIR "${CMAKE_BINARY_DIR}/libvorbis")
    SET(_Vorbis_SOURCE_DIR "${CMAKE_BINARY_DIR}/libvorbis_src")
    
    SET(_WITH_VORBIS --with-vorbis=${_Vorbis_DIR})

    SET(_Vorbis_INSTALL_COMMAND1 "")
    SET(_Vorbis_INSTALL_COMMAND2 "")
    SET(_Vorbis_INSTALL_COMMAND3 "")
    SET(_Vorbis_INSTALL_COMMAND4 "")
    IF(MSVC) 
        SET(SOLUTION win32/VS2010/vorbis_static.sln)
        SET(_Vorbis_INSTALL_COMMAND1 ${CMAKE_VS_DEVENV_COMMAND} ${SOLUTION} /Upgrade )
        SET(_Vorbis_INSTALL_COMMAND2 ${CMAKE_VS_MSBUILD_COMMAND} ${SOLUTION} /t:libvorbisfile /clp:ErrorsOnly /p:OutDir=${_Vorbis_DIR}/lib/ )
        SET(_Vorbis_INSTALL_COMMAND3 ${CMAKE_COMMAND} -E make_directory ${_Vorbis_DIR}/include/vorbis)
        SET(_Vorbis_INSTALL_COMMAND4 ${CMAKE_COMMAND} -E copy include/vorbis/codec.h include/vorbis/vorbisfile.h ${_Vorbis_DIR}/include/vorbis)
    ELSE()
        SET(_Vorbis_INSTALL_COMMAND1 ./autogen.sh )
        SET(_Vorbis_INSTALL_COMMAND2 ./configure ${_WITH_OGG} --prefix=${_Vorbis_DIR} )
        SET(_Vorbis_INSTALL_COMMAND3 ${CMAKE_MAKE_PROGRAM} install -l )
    ENDIF()

    ExternalProject_Add(
        ThirdParty_Vorbis
        GIT_REPOSITORY      "https://github.com/xiph/vorbis.git"
        GIT_TAG             "v1.3.6"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${_Vorbis_SOURCE_DIR}"
        BUILD_IN_SOURCE     TRUE
        CONFIGURE_COMMAND   ""
        BUILD_COMMAND       ""
        INSTALL_COMMAND     ${_Vorbis_INSTALL_COMMAND1}
        COMMAND             ${_Vorbis_INSTALL_COMMAND2}
        COMMAND             ${_Vorbis_INSTALL_COMMAND3}
        COMMAND             ${_Vorbis_INSTALL_COMMAND4}
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )
    
    ADD_DEPENDENCIES(ThirdParty_Vorbis ThirdParty_Ogg)

    SET(_Theora_DIR "${CMAKE_BINARY_DIR}/libtheora")
    SET(_Theora_SOURCE_DIR "${CMAKE_BINARY_DIR}/libtheora_src")

    SET(_Theora_INSTALL_COMMAND1 "")
    SET(_Theora_INSTALL_COMMAND2 "")
    SET(_Theora_INSTALL_COMMAND3 "")
    SET(_Theora_INSTALL_COMMAND4 "")
    IF(MSVC) 
        SET(SOLUTION win32/VS2010/libtheora_static.sln)
        SET(_Theora_INSTALL_COMMAND1 ${CMAKE_VS_DEVENV_COMMAND} ${SOLUTION} /Upgrade )
        SET(_Theora_INSTALL_COMMAND2 ${CMAKE_VS_MSBUILD_COMMAND} ${SOLUTION} /t:libtheora_static /clp:ErrorsOnly /p:OutDir=${_Theora_DIR}/lib/ )
        SET(_Theora_INSTALL_COMMAND3 ${CMAKE_COMMAND} -E make_directory ${_Theora_DIR}/include/theora)
        SET(_Theora_INSTALL_COMMAND4 ${CMAKE_COMMAND} -E copy include/theora/codec.h include/theora/theora.h ${_Theora_DIR}/include/theora)
    ELSE()
        SET(_Theora_INSTALL_COMMAND1 ./autogen.sh ${_WITH_OGG} ${_WITH_VORBIS} --prefix=${_Theora_DIR} )
        SET(_Theora_INSTALL_COMMAND2 ${CMAKE_MAKE_PROGRAM} install -l )
    ENDIF()

    ExternalProject_Add(
        ThirdParty_Theora
        GIT_REPOSITORY      "https://github.com/xiph/theora.git"
        GIT_TAG             "v1.2.0alpha1"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${_Theora_SOURCE_DIR}"
        BUILD_IN_SOURCE     TRUE
        CONFIGURE_COMMAND   ""
        BUILD_COMMAND       ""
        INSTALL_COMMAND     ${_Theora_INSTALL_COMMAND1}
        COMMAND             ${_Theora_INSTALL_COMMAND2}
        COMMAND             ${_Theora_INSTALL_COMMAND3}
        COMMAND             ${_Theora_INSTALL_COMMAND4}
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )
    
    ADD_DEPENDENCIES(ThirdParty_Theora ThirdParty_Ogg ThirdParty_Vorbis)
ENDIF()

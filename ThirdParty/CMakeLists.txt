CMAKE_MINIMUM_REQUIRED(VERSION 3.12 FATAL_ERROR)

LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/../CMake")

# Include CMAKE_POLICY decisions
INCLUDE(Policies)
INCLUDE(ExternalProject)

PROJECT(ThirdParty)

# glm

SET(glm_ROOT       "${CMAKE_BINARY_DIR}/glm")
SET(glm_SOURCE_DIR "${CMAKE_BINARY_DIR}/glm_src")

FIND_PACKAGE(glm QUIET)

IF(NOT glm_FOUND)
    ExternalProject_Add(
        ThirdParty_glm
        GIT_REPOSITORY      "https://github.com/g-truc/glm.git"
        GIT_TAG             "0.9.9.3"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${glm_SOURCE_DIR}"
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${glm_ROOT}
                            -DGLM_TEST_ENABLE=OFF
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )
ENDIF()

# nlohmann_json

SET(nlohmann_json_ROOT       "${CMAKE_BINARY_DIR}/nlohmann_json")
SET(nlohmann_json_SOURCE_DIR "${CMAKE_BINARY_DIR}/nlohmann_json_src")

FIND_PACKAGE(nlohmann_json QUIET)

IF(NOT nlohmann_json_FOUND)
    ExternalProject_Add(
        ThirdParty_nlohmann_json
        GIT_REPOSITORY      "https://github.com/nlohmann/json.git"
        GIT_TAG             "master"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${nlohmann_json_SOURCE_DIR}"
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${nlohmann_json_ROOT}
                            -DJSON_BuildTests=OFF
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )
ENDIF()

# GTest

IF(BUILD_TESTS)
    SET(GTest_ROOT       "${CMAKE_BINARY_DIR}/GTest")
    SET(GTest_SOURCE_DIR "${CMAKE_BINARY_DIR}/GTest_src")
    SET(GTest_BINARY_DIR "${CMAKE_BINARY_DIR}/GTest_bin")

    FIND_PACKAGE(GTest QUIET)

    IF(NOT GTEST_FOUND)

        ExternalProject_Add(
            ThirdParty_GTest
            GIT_REPOSITORY      "https://github.com/google/googletest"
            GIT_TAG             "v1.10.x"
            GIT_SHALLOW         TRUE
            SOURCE_DIR          "${GTest_SOURCE_DIR}"
            BINARY_DIR          "${GTest_BINARY_DIR}"
            CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${GTest_ROOT}
                                -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            UPDATE_COMMAND      ""
            TEST_COMMAND        ""
        )
    ENDIF()
ENDIF()

# SDL2

SET(SDL2_ROOT       "${CMAKE_BINARY_DIR}/SDL2")
SET(SDL2_SOURCE_DIR "${CMAKE_BINARY_DIR}/SDL2_src")
SET(SDL2_BINARY_DIR "${CMAKE_BINARY_DIR}/SDL2_bin")

FIND_PACKAGE(SDL2 QUIET)

IF(NOT SDL2_FOUND AND NEED_SDL2)
    ExternalProject_Add(
        ThirdParty_SDL2
        GIT_REPOSITORY      "https://github.com/spurious/SDL-mirror.git"
        GIT_TAG             "master"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${SDL2_SOURCE_DIR}"
        BINARY_DIR          "${SDL2_BINARY_DIR}"
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${SDL2_ROOT}
                            -DBUILD_SHARED_LIBS=ON
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )
ENDIF()

# Python3

SET(Python3_ROOT       "${CMAKE_BINARY_DIR}/Python3")
SET(Python3_SOURCE_DIR "${CMAKE_BINARY_DIR}/Python3_src")

FIND_PACKAGE(Python3 QUIET COMPONENTS Development)

IF(NOT Python3_FOUND)

    SET(_Python3_INSTALL_COMMAND1 "")
    SET(_Python3_INSTALL_COMMAND2 "")

    IF(WIN32)
        IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
            SET(ARCH "x64")
        ELSEIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
            SET(ARCH "Win32")
        ENDIF()
        SET(_Python3_INSTALL_COMMAND1 cmd /c PCbuild\\build.bat -c Debug -t Build -p ${ARCH})
        SET(_Python3_INSTALL_COMMAND2 cmd /c PCbuild\\build.bat -c Release -t Build -p ${ARCH})
    ELSE()
        SET(_Python3_INSTALL_COMMAND1 ./configure --prefix=${Python3_ROOT} --enable-shared --enable-optimizations)
        SET(_Python3_INSTALL_COMMAND2 ${CMAKE_MAKE_PROGRAM} -l libpython3.so install)
    ENDIF()

    ExternalProject_Add(
        ThirdParty_Python3
        GIT_REPOSITORY      "https://github.com/python/cpython.git"
        GIT_TAG             "3.8"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${Python3_SOURCE_DIR}"
        BUILD_IN_SOURCE     TRUE
        CONFIGURE_COMMAND   ""
        BUILD_COMMAND       ""
        UPDATE_COMMAND      ""
        INSTALL_COMMAND     "${_Python3_INSTALL_COMMAND1}"
        COMMAND             "${_Python3_INSTALL_COMMAND2}"
        TEST_COMMAND        ""
    )
ENDIF()

# assimp

SET(assimp_ROOT       "${CMAKE_BINARY_DIR}/assimp")
SET(assimp_SOURCE_DIR "${CMAKE_BINARY_DIR}/assimp_src")
SET(assimp_BINARY_DIR "${CMAKE_BINARY_DIR}/assimp_bin")

FIND_PACKAGE(assimp 5.0 QUIET)

IF(NOT assimp_FOUND AND NEED_ASSIMP)
    ExternalProject_Add(
        ThirdParty_assimp
        GIT_REPOSITORY      "https://github.com/assimp/assimp.git"
        GIT_TAG             "master"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${assimp_SOURCE_DIR}"
        BINARY_DIR          "${assimp_BINARY_DIR}"
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${assimp_ROOT}
                            -DASSIMP_BUILD_ASSIMP_TOOLS=OFF
                            -DASSIMP_BUILD_SAMPLES=OFF
                            -DASSIMP_BUILD_TESTS=OFF
                            -DBUILD_DOCS=OFF
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )
ENDIF()

# OpenAL

SET(OpenAL_ROOT       "${CMAKE_BINARY_DIR}/OpenAL")
SET(OpenAL_SOURCE_DIR "${CMAKE_BINARY_DIR}/OpenAL_src")
SET(OpenAL_BINARY_DIR "${CMAKE_BINARY_DIR}/OpenAL_bin")

FIND_PACKAGE(OpenAL QUIET)

IF(NOT OpenAL_FOUND AND NEED_OPENAL)
    ExternalProject_Add(
        ThirdParty_OpenAL
        GIT_REPOSITORY      "https://github.com/kcat/openal-soft.git"
        GIT_TAG             "v1.19"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${OpenAL_SOURCE_DIR}"
        BINARY_DIR          "${OpenAL_BINARY_DIR}"
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${OpenAL_ROOT}
                            -DALSOFT_UTILS=OFF
                            -DALSOFT_EXAMPLES=OFF
                            -DALSOFT_TESTS=OFF
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )
ENDIF()

# Ogg, Vorbis, Theora

SET(Ogg_ROOT          "${CMAKE_BINARY_DIR}/libogg")
SET(Ogg_SOURCE_DIR    "${CMAKE_BINARY_DIR}/libogg_src")

SET(Vorbis_ROOT       "${CMAKE_BINARY_DIR}/libvorbis")
SET(Vorbis_SOURCE_DIR "${CMAKE_BINARY_DIR}/libvorbis_src")

SET(Theora_ROOT       "${CMAKE_BINARY_DIR}/libtheora")
SET(Theora_SOURCE_DIR "${CMAKE_BINARY_DIR}/libtheora_src")

FIND_PACKAGE(Ogg QUIET COMPONENTS Vorbis Theora)

IF(NOT Ogg_FOUND AND NEED_OGG)
    SET(_WITH_OGG --with-ogg=${Ogg_ROOT})

    SET(_Ogg_INSTALL_COMMAND1 "")
    SET(_Ogg_INSTALL_COMMAND2 "")
    SET(_Ogg_INSTALL_COMMAND3 "")
    SET(_Ogg_INSTALL_COMMAND4 "")
    IF(MSVC) 
        SET(SOLUTION win32/VS2015/libogg_static.sln)
        SET(_Ogg_INSTALL_COMMAND1 ${CMAKE_VS_DEVENV_COMMAND} ${SOLUTION} /Upgrade )
        SET(_Ogg_INSTALL_COMMAND2 ${CMAKE_VS_MSBUILD_COMMAND} ${SOLUTION} /t:libogg_static /clp:ErrorsOnly /p:OutDir=${Ogg_ROOT}/lib/ )
        SET(_Ogg_INSTALL_COMMAND3 ${CMAKE_COMMAND} -E make_directory ${Ogg_ROOT}/include/ogg)
        SET(_Ogg_INSTALL_COMMAND4 ${CMAKE_COMMAND} -E copy include/ogg/ogg.h include/ogg/os_types.h ${Ogg_ROOT}/include/ogg)
    ELSE()
        SET(_Ogg_INSTALL_COMMAND1 ./autogen.sh )
        SET(_Ogg_INSTALL_COMMAND2 ./configure --prefix=${Ogg_ROOT} )
        SET(_Ogg_INSTALL_COMMAND3 ${CMAKE_MAKE_PROGRAM} install -l )
    ENDIF()

    ExternalProject_Add(
        ThirdParty_Ogg
        GIT_REPOSITORY      "https://github.com/xiph/ogg.git"
        GIT_TAG             "v1.3.4"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${Ogg_SOURCE_DIR}"
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
    
    SET(_WITH_VORBIS --with-vorbis=${Vorbis_ROOT})

    SET(_Vorbis_INSTALL_COMMAND1 "")
    SET(_Vorbis_INSTALL_COMMAND2 "")
    SET(_Vorbis_INSTALL_COMMAND3 "")
    SET(_Vorbis_INSTALL_COMMAND4 "")
    IF(MSVC) 
        SET(SOLUTION win32/VS2010/vorbis_static.sln)
        SET(_Vorbis_INSTALL_COMMAND1 ${CMAKE_VS_DEVENV_COMMAND} ${SOLUTION} /Upgrade )
        SET(_Vorbis_INSTALL_COMMAND2 ${CMAKE_VS_MSBUILD_COMMAND} ${SOLUTION} /t:libvorbisfile /clp:ErrorsOnly /p:OutDir=${Vorbis_ROOT}/lib/ )
        SET(_Vorbis_INSTALL_COMMAND3 ${CMAKE_COMMAND} -E make_directory ${Vorbis_ROOT}/include/vorbis)
        SET(_Vorbis_INSTALL_COMMAND4 ${CMAKE_COMMAND} -E copy include/vorbis/codec.h include/vorbis/vorbisfile.h ${Vorbis_ROOT}/include/vorbis)
    ELSE()
        SET(_Vorbis_INSTALL_COMMAND1 ./autogen.sh )
        SET(_Vorbis_INSTALL_COMMAND2 ./configure ${_WITH_OGG} --prefix=${Vorbis_ROOT} )
        SET(_Vorbis_INSTALL_COMMAND3 ${CMAKE_MAKE_PROGRAM} install -l )
    ENDIF()

    ExternalProject_Add(
        ThirdParty_Vorbis
        GIT_REPOSITORY      "https://github.com/xiph/vorbis.git"
        GIT_TAG             "v1.3.6"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${Vorbis_SOURCE_DIR}"
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

    SET(_Theora_INSTALL_COMMAND1 "")
    SET(_Theora_INSTALL_COMMAND2 "")
    SET(_Theora_INSTALL_COMMAND3 "")
    SET(_Theora_INSTALL_COMMAND4 "")
    IF(MSVC) 
        SET(SOLUTION win32/VS2010/libtheora_static.sln)
        SET(_Theora_INSTALL_COMMAND1 ${CMAKE_VS_DEVENV_COMMAND} ${SOLUTION} /Upgrade )
        SET(_Theora_INSTALL_COMMAND2 ${CMAKE_VS_MSBUILD_COMMAND} ${SOLUTION} /t:libtheora_static /clp:ErrorsOnly /p:OutDir=${Theora_ROOT}/lib/ )
        SET(_Theora_INSTALL_COMMAND3 ${CMAKE_COMMAND} -E make_directory ${Theora_ROOT}/include/theora)
        SET(_Theora_INSTALL_COMMAND4 ${CMAKE_COMMAND} -E copy include/theora/codec.h include/theora/theora.h ${Theora_ROOT}/include/theora)
    ELSE()
        SET(_Theora_INSTALL_COMMAND1 ./autogen.sh ${_WITH_OGG} ${_WITH_VORBIS} --prefix=${Theora_ROOT} )
        SET(_Theora_INSTALL_COMMAND2 ${CMAKE_MAKE_PROGRAM} install -l )
    ENDIF()

    ExternalProject_Add(
        ThirdParty_Theora
        GIT_REPOSITORY      "https://github.com/xiph/theora.git"
        GIT_TAG             "v1.1.1"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${Theora_SOURCE_DIR}"
        BUILD_IN_SOURCE     TRUE
        CONFIGURE_COMMAND   ""
        BUILD_COMMAND       ""
        INSTALL_COMMAND     ${_Theora_INSTALL_COMMAND1}
        COMMAND             ${_Theora_INSTALL_COMMAND2}
        COMMAND             ${_Theora_INSTALL_COMMAND3}
        COMMAND             ${_Theora_INSTALL_COMMAND4}
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
        PATCH_COMMAND       ${GIT_EXECUTABLE} apply ${CMAKE_SOURCE_DIR}/libtheora-png_sizeof.patch
    )
    
    ADD_DEPENDENCIES(ThirdParty_Theora ThirdParty_Ogg ThirdParty_Vorbis)
ENDIF()

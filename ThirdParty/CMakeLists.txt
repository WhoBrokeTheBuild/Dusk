CMAKE_MINIMUM_REQUIRED(VERSION 3.15 FATAL_ERROR)

LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/../CMake")

INCLUDE(Policies)
INCLUDE(ExternalProject)

PROJECT(ThirdParty)

# glm

SET(glm_ROOT       ${CMAKE_BINARY_DIR}/glm/cmake)
SET(glm_SOURCE_DIR ${CMAKE_BINARY_DIR}/glm)

FIND_PACKAGE(glm CONFIG QUIET)

IF(NOT glm_FOUND OR NOT TARGET glm::glm)
    ExternalProject_Add(
        ThirdParty_glm
        URL                 "https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip"
        URL_HASH            MD5=69895110052f0d711c9c54fbf385f6f5
        SOURCE_DIR          ${glm_SOURCE_DIR}
        CONFIGURE_COMMAND   ""
        BUILD_COMMAND       ""
        INSTALL_COMMAND     ""
        UPDATE_COMMAND      ""
    )
ENDIF()

# GSL

SET(GSL_ROOT       ${CMAKE_BINARY_DIR}/GSL)
SET(GSL_SOURCE_DIR ${CMAKE_BINARY_DIR}/GSL_src)

FIND_PACKAGE(GSL QUIET)

IF(NOT GSL_FOUND)
    ExternalProject_Add(
        ThirdParty_GSL
        URL                 "https://github.com/microsoft/GSL/archive/v3.1.0.tar.gz"
        URL_HASH            MD5=b6910c54113f921b03dc06642cf7f11c
        SOURCE_DIR          ${GSL_SOURCE_DIR}
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${GSL_ROOT}
                            -DGSL_TEST=OFF
        BUILD_COMMAND       ""
        UPDATE_COMMAND      ""
    )
ENDIF()

# nlohmann_json

SET(nlohmann_json_ROOT       ${CMAKE_BINARY_DIR}/nlohmann_json)
SET(nlohmann_json_SOURCE_DIR ${CMAKE_BINARY_DIR}/nlohmann_json_src)

FIND_PACKAGE(nlohmann_json CONFIG QUIET)

IF(NOT nlohmann_json_FOUND)
    ExternalProject_Add(
        ThirdParty_nlohmann_json
        URL                 "https://github.com/nlohmann/json/archive/v3.9.1.tar.gz"
        URL_HASH            MD5=e386222fb57dd2fcb8a7879fc016d037
        SOURCE_DIR          ${nlohmann_json_SOURCE_DIR}
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${nlohmann_json_ROOT}
                            -DJSON_BuildTests=OFF
        BUILD_COMMAND       ""
        UPDATE_COMMAND      ""
    )
ENDIF()

# GTest

IF(NEED_GTest)
    SET(GTest_ROOT       ${CMAKE_BINARY_DIR}/GTest)
    SET(GTest_SOURCE_DIR ${CMAKE_BINARY_DIR}/GTest_src)
    SET(GTest_BINARY_DIR ${CMAKE_BINARY_DIR}/GTest_bin)

    FIND_PACKAGE(GTest QUIET)

    IF(NOT GTEST_FOUND)
        ExternalProject_Add(
            ThirdParty_GTest
            URL                 "https://github.com/google/googletest/archive/release-1.10.0.tar.gz"
            URL_HASH            MD5=ecd1fa65e7de707cd5c00bdac56022cd
            SOURCE_DIR          ${GTest_SOURCE_DIR}
            BINARY_DIR          ${GTest_BINARY_DIR}
            CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${GTest_ROOT}
                                -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            BUILD_COMMAND       ""
            UPDATE_COMMAND      ""
            TEST_COMMAND        ""
            INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install --config Release
        )
    ENDIF()
ENDIF()

# SDL2

IF(NEED_SDL2)
    SET(SDL2_ROOT       ${CMAKE_BINARY_DIR}/SDL2)
    SET(SDL2_SOURCE_DIR ${CMAKE_BINARY_DIR}/SDL2_src)
    SET(SDL2_BINARY_DIR ${CMAKE_BINARY_DIR}/SDL2_bin)

    FIND_PACKAGE(SDL2 2.0.6 CONFIG QUIET)

    IF(NOT SDL2_FOUND)
        ExternalProject_Add(
            ThirdParty_SDL2
            URL                 "https://www.libsdl.org/release/SDL2-2.0.12.tar.gz"
            URL_HASH            MD5=783b6f2df8ff02b19bb5ce492b99c8ff
            SOURCE_DIR          ${SDL2_SOURCE_DIR}
            BINARY_DIR          ${SDL2_BINARY_DIR}
            CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${SDL2_ROOT}
                                -DBUILD_SHARED_LIBS=ON
            BUILD_COMMAND       ""
            UPDATE_COMMAND      ""
            TEST_COMMAND        ""
            INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install --config Release
            COMMAND             ${CMAKE_COMMAND} --build . --target install --config Debug
        )
    ENDIF()
ENDIF()

# Python3

IF(WIN32)
    SET(Python3_ROOT_DIR   ${CMAKE_BINARY_DIR}/Python3)
    SET(Python3_SOURCE_DIR ${Python3_ROOT_DIR})
ELSE()
    SET(Python3_ROOT_DIR   ${CMAKE_BINARY_DIR}/Python3)
    SET(Python3_SOURCE_DIR ${CMAKE_BINARY_DIR}/Python3_src)
ENDIF()

FIND_PACKAGE(Python3 3.8 QUIET COMPONENTS Development)

IF(NOT Python3_FOUND)
    SET(_Python3_INSTALL_COMMAND1 "")
    SET(_Python3_INSTALL_COMMAND2 "")

    IF(WIN32)
        MESSAGE(SEND_ERROR "Unable to find Python3, install to continue https://www.python.org/downloads/windows/")
    ELSE()
        SET(_Python3_INSTALL_COMMAND1 ./configure --prefix=${Python3_ROOT_DIR} --enable-shared --enable-optimizations)
        SET(_Python3_INSTALL_COMMAND2 ${CMAKE_MAKE_PROGRAM} -l libpython3.so install)
    ENDIF()

    ExternalProject_Add(
        ThirdParty_Python3
        URL                 "https://www.python.org/ftp/python/3.8.5/Python-3.8.5.tgz"
        URL_HASH            MD5=e2f52bcf531c8cc94732c0b6ff933ff0
        SOURCE_DIR          ${Python3_SOURCE_DIR}
        BUILD_IN_SOURCE     TRUE
        CONFIGURE_COMMAND   ""
        BUILD_COMMAND       ""
        UPDATE_COMMAND      ""
        INSTALL_COMMAND     ${_Python3_INSTALL_COMMAND1}
        COMMAND             ${_Python3_INSTALL_COMMAND2}
        TEST_COMMAND        ""
    )
ENDIF()

# OpenAL

IF(NEED_OpenAL)
    SET(OpenAL_ROOT       ${CMAKE_BINARY_DIR}/OpenAL)
    SET(OpenAL_SOURCE_DIR ${CMAKE_BINARY_DIR}/OpenAL_src)
    SET(OpenAL_BINARY_DIR ${CMAKE_BINARY_DIR}/OpenAL_bin)

    FIND_PACKAGE(OpenAL CONFIG QUIET)

    IF(NOT OpenAL_FOUND)
        ExternalProject_Add(
            ThirdParty_OpenAL
            URL                 "https://github.com/kcat/openal-soft/archive/openal-soft-1.19.0.tar.gz"
            URL_HASH            MD5=a98737cc8fe65eb9c91b82c719c6465f
            SOURCE_DIR          ${OpenAL_SOURCE_DIR}
            BINARY_DIR          ${OpenAL_BINARY_DIR}
            CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${OpenAL_ROOT}
                                -DALSOFT_UTILS=OFF
                                -DALSOFT_EXAMPLES=OFF
                                -DALSOFT_TESTS=OFF
            BUILD_COMMAND       ""
            UPDATE_COMMAND      ""
            TEST_COMMAND        ""
            INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install --config Release
        )
    ENDIF()
ENDIF()

# freetype

IF(NEED_freetype)
    SET(freetype_ROOT           ${CMAKE_BINARY_DIR}/freetype)
    SET(freetype_SOURCE_DIR     ${CMAKE_BINARY_DIR}/freetype_src)
    SET(freetype_BINARY_DIR     ${CMAKE_BINARY_DIR}/freetype_bin)
    
    FIND_PACKAGE(freetype CONFIG QUIET)

    IF(NOT freetype_FOUND)
        ExternalProject_Add(
            ThirdParty_freetype
            URL                 "https://github.com/aseprite/freetype2/archive/VER-2-10-0.tar.gz"
            URL_HASH            MD5=33a87b91db6334c02e5b2a6c98604aa8
            SOURCE_DIR          ${freetype_SOURCE_DIR}
            BINARY_DIR          ${freetype_BINARY_DIR}
            CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${freetype_ROOT}
            BUILD_COMMAND       ""
            UPDATE_COMMAND      ""
            TEST_COMMAND        ""
            INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install --config Release
        )
    ENDIF()
ENDIF()

# Ogg, Vorbis, Theora

IF(NEED_Ogg)
    SET(Ogg_ROOT          ${CMAKE_BINARY_DIR}/libogg)
    SET(Ogg_SOURCE_DIR    ${CMAKE_BINARY_DIR}/libogg_src)

    SET(Vorbis_ROOT       ${CMAKE_BINARY_DIR}/libvorbis)
    SET(Vorbis_SOURCE_DIR ${CMAKE_BINARY_DIR}/libvorbis_src)

    SET(Theora_ROOT       ${CMAKE_BINARY_DIR}/libtheora)
    SET(Theora_SOURCE_DIR ${CMAKE_BINARY_DIR}/libtheora_src)

    FIND_PACKAGE(Ogg QUIET COMPONENTS Vorbis Theora)

    IF(NOT Ogg_FOUND)
        SET(_WITH_OGG --with-ogg=${Ogg_ROOT})

        SET(_Ogg_INSTALL_COMMAND1 "")
        SET(_Ogg_INSTALL_COMMAND2 "")
        SET(_Ogg_INSTALL_COMMAND3 "")
        SET(_Ogg_INSTALL_COMMAND4 "")
        IF(MSVC) 
            SET(_Ogg_SOLUTION win32/VS2015/libogg.sln)
            SET(_Ogg_INSTALL_COMMAND1 ${CMAKE_VS_DEVENV_COMMAND} ${_Ogg_SOLUTION} /Upgrade )
            SET(_Ogg_INSTALL_COMMAND2 ${CMAKE_VS_MSBUILD_COMMAND} ${_Ogg_SOLUTION} /t:libogg /clp:ErrorsOnly /p:OutDir=${Ogg_ROOT}/lib/ )
            SET(_Ogg_INSTALL_COMMAND3 ${CMAKE_COMMAND} -E make_directory ${Ogg_ROOT}/include/ogg)
            SET(_Ogg_INSTALL_COMMAND4 ${CMAKE_COMMAND} -E copy include/ogg/ogg.h include/ogg/os_types.h ${Ogg_ROOT}/include/ogg)
        ELSE()
            SET(_Ogg_INSTALL_COMMAND1 ./autogen.sh )
            SET(_Ogg_INSTALL_COMMAND2 ./configure --prefix=${Ogg_ROOT} )
            SET(_Ogg_INSTALL_COMMAND3 ${CMAKE_MAKE_PROGRAM} install -l )
        ENDIF()

        ExternalProject_Add(
            ThirdParty_Ogg
            URL                 "https://github.com/xiph/ogg/releases/download/v1.3.4/libogg-1.3.4.tar.gz"
            URL_HASH            MD5=b9a66c80bdf45363605e4aa75fa951a8
            SOURCE_DIR          ${Ogg_SOURCE_DIR}
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
            SET(_Vorbis_SOLUTION win32/VS2010/vorbis_static.sln)
            SET(_Vorbis_INSTALL_COMMAND1 ${CMAKE_VS_DEVENV_COMMAND} ${_Vorbis_SOLUTION} /Upgrade )
            SET(_Vorbis_INSTALL_COMMAND2 ${CMAKE_VS_MSBUILD_COMMAND} ${_Vorbis_SOLUTION} /t:libvorbisfile /clp:ErrorsOnly /p:OutDir=${Vorbis_ROOT}/lib/ )
            SET(_Vorbis_INSTALL_COMMAND3 ${CMAKE_COMMAND} -E make_directory ${Vorbis_ROOT}/include/vorbis)
            SET(_Vorbis_INSTALL_COMMAND4 ${CMAKE_COMMAND} -E copy include/vorbis/codec.h include/vorbis/vorbisfile.h ${Vorbis_ROOT}/include/vorbis)
        ELSE()
            SET(_Vorbis_INSTALL_COMMAND1 ./autogen.sh )
            SET(_Vorbis_INSTALL_COMMAND2 ./configure ${_WITH_OGG} --prefix=${Vorbis_ROOT} )
            SET(_Vorbis_INSTALL_COMMAND3 ${CMAKE_MAKE_PROGRAM} install -l )
        ENDIF()

        ExternalProject_Add(
            ThirdParty_Vorbis
            URL                 "https://github.com/xiph/vorbis/releases/download/v1.3.7/libvorbis-1.3.7.tar.gz"
            URL_HASH            MD5=9b8034da6edc1a17d18b9bc4542015c7
            SOURCE_DIR          ${Vorbis_SOURCE_DIR}
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

        SET(_Theora_PATCH ${CMAKE_SOURCE_DIR}/libtheora-png_sizeof.patch)

        SET(_Theora_INSTALL_COMMAND1 "")
        SET(_Theora_INSTALL_COMMAND2 "")
        SET(_Theora_INSTALL_COMMAND3 "")
        SET(_Theora_INSTALL_COMMAND4 "")
        IF(MSVC) 
            SET(_Theora_SOLUTION win32/VS2008/libtheora_static.sln)
            SET(_Theora_PATCH_COMMAND ${GIT_EXECUTABLE} apply --ignore-space-change --ignore-whitespace -v ${_Theora_PATCH} || ver > nul)
            SET(_Theora_INSTALL_COMMAND1 ${CMAKE_VS_DEVENV_COMMAND} ${_Theora_SOLUTION} /Upgrade )
            SET(_Theora_INSTALL_COMMAND2 ${CMAKE_VS_MSBUILD_COMMAND} ${_Theora_SOLUTION} /t:libtheora_static /clp:ErrorsOnly /p:OutDir=${Theora_ROOT}/lib/ )
            SET(_Theora_INSTALL_COMMAND3 ${CMAKE_COMMAND} -E make_directory ${Theora_ROOT}/include/theora)
            SET(_Theora_INSTALL_COMMAND4 ${CMAKE_COMMAND} -E copy include/theora/codec.h include/theora/theora.h ${Theora_ROOT}/include/theora)
        ELSE()
            SET(_Theora_PATCH_COMMAND patch -p0 --forward < ${_Theora_PATCH} || true)
            SET(_Theora_INSTALL_COMMAND1 ./autogen.sh ${_WITH_OGG} ${_WITH_VORBIS} --prefix=${Theora_ROOT} )
            SET(_Theora_INSTALL_COMMAND2 ${CMAKE_MAKE_PROGRAM} install -l )
        ENDIF()

        ExternalProject_Add(
            ThirdParty_Theora
            URL                "https://github.com/xiph/theora/archive/v1.1.1.tar.gz"
            URL_HASH            MD5=c19c149cde55d631902a5396ea921bf3
            SOURCE_DIR          ${Theora_SOURCE_DIR}
            BUILD_IN_SOURCE     TRUE
            CONFIGURE_COMMAND   ""
            BUILD_COMMAND       ""
            INSTALL_COMMAND     ${_Theora_INSTALL_COMMAND1}
            COMMAND             ${_Theora_INSTALL_COMMAND2}
            COMMAND             ${_Theora_INSTALL_COMMAND3}
            COMMAND             ${_Theora_INSTALL_COMMAND4}
            UPDATE_COMMAND      ""
            TEST_COMMAND        ""
            PATCH_COMMAND       ${_Theora_PATCH_COMMAND}
            COMMAND             
        )
        
        ADD_DEPENDENCIES(ThirdParty_Theora ThirdParty_Ogg ThirdParty_Vorbis)
    ENDIF()
ENDIF()

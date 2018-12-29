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

# Assimp

FIND_PACKAGE(Assimp QUIET)

IF(NOT Assimp_FOUND)
    SET(_ASSIMP_DIR "${CMAKE_BINARY_DIR}/Assimp")
    SET(_ASSIMP_SOURCE_DIR "${CMAKE_BINARY_DIR}/Assimp_src")
    SET(_ASSIMP_BINARY_DIR "${CMAKE_BINARY_DIR}/Assimp_bin")

    SET(_ASSIMP_CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${_ASSIMP_DIR}
        -DASSIMP_BUILD_ASSIMP_TOOLS=OFF
        -DASSIMP_BUILD_SAMPLES=OFF
        -DASSIMP_BUILD_TESTS=OFF
    )

    IF(WIN32)
        SET(_ASSIMP_CMAKE_ARGS
            ${_ASSIMP_CMAKE_ARGS}
            -DASSIMP_BUILD_ZLIB=${TRUE_WHEN_WIN32}
        )
    ENDIF()

    ExternalProject_Add(
        ThirdParty_Assimp
        GIT_REPOSITORY      "https://github.com/assimp/assimp"
        GIT_TAG             "master"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${_ASSIMP_SOURCE_DIR}"
        BINARY_DIR          "${_ASSIMP_BINARY_DIR}"
        CMAKE_ARGS          ${_ASSIMP_CMAKE_ARGS}
        BUILD_COMMAND       ""
        INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install --config Release
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
    )
ENDIF()
CMAKE_MINIMUM_REQUIRED(VERSION 3.12 FATAL_ERROR)

# Allow for custom CMake modules
LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/CMake")

# Include CMAKE_POLICY decisions
INCLUDE(Policies)

###
### Project
###

PROJECT(Dusk)

###
### Options
###

OPTION(
    VERBOSE_LOGGING 
    "Build Dusk with Verbose Logging enabled" 
    OFF
)
IF(VERBOSE_LOGGING)
    ADD_DEFINITIONS(-DDUSK_ENABLE_VERBOSE_LOGGING)
ENDIF()

OPTION(
    BINARY_SHADERS 
    "Build Dusk with Binary Shader support" 
    OFF
)
IF(BINARY_SHADERS)
    ADD_DEFINITIONS(-DDUSK_ENABLE_BINARY_SHADERS)
ENDIF()

OPTION(BUILD_TESTS "Build Tests" OFF)
IF(BUILD_TESTS)
    ENABLE_TESTING()
ENDIF()

OPTION(BUILD_DEMOS "Build Dusk Demo Projects" ON)

OPTION(BUILD_THIRDPARTY "Build Dusk ThirdParty Dependencies" ON)

OPTION(BUILD_PACKAGES "Build Dusk Packages" OFF)

OPTION(NEED_SDL2 "" ON)
OPTION(NEED_OPENAL "" OFF)
OPTION(NEED_ASSIMP "" OFF)
OPTION(NEED_OGG "" OFF)

###
### Configuration
###

# Allow for custom organization of files in VisualStudio
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Get git hash
INCLUDE(GetGitRevisionDescription)
GET_GIT_HEAD_REVISION(GIT_REFSPEC PROJECT_VERSION_GIT_HASH)
STRING(SUBSTRING "${PROJECT_VERSION_GIT_HASH}" 0 8 PROJECT_VERSION_GIT_HASH)

# Parse version from git tags
GIT_DESCRIBE(VERSION --tags)
STRING(REGEX REPLACE "^v([0-9]+)\\..*" "\\1" PROJECT_VERSION_MAJOR "${VERSION}")
STRING(REGEX REPLACE "^v[0-9]+\\.([0-9]+).*" "\\1" PROJECT_VERSION_MINOR "${VERSION}")
STRING(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" PROJECT_VERSION_PATCH "${VERSION}")
SET(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# Configure Visual Studio exception handling
IF(MSVC)
    # Disable Default Exception Handling
    STRING(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    # Disable Basic Runtime Checks
    STRING(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS}")
    STRING(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS}")
ENDIF()

# Don't use the install RPATH when building
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# Limit Visual Studio Configuration Types
IF(CMAKE_CONFIGURATION_TYPES)
    SET(CMAKE_CONFIGURATION_TYPES Debug Release)
ENDIF()

###
### Third Party Dependencies
###

IF(BUILD_THIRDPARTY)
    SET(THIRDPARTY_SOURCE_DIR "${CMAKE_SOURCE_DIR}/ThirdParty")
    SET(THIRDPARTY_BINARY_DIR "${CMAKE_BINARY_DIR}/ThirdParty")

    FILE(MAKE_DIRECTORY "${THIRDPARTY_BINARY_DIR}")

    LIST(APPEND CMAKE_PREFIX_PATH
        "${THIRDPARTY_BINARY_DIR}/glm"
        "${THIRDPARTY_BINARY_DIR}/nlohmann_json"
        "${THIRDPARTY_BINARY_DIR}/SDL2"
        "${THIRDPARTY_BINARY_DIR}/Python3"
        "${THIRDPARTY_BINARY_DIR}/OpenAL"
        "${THIRDPARTY_BINARY_DIR}/assimp"
        "${THIRDPARTY_BINARY_DIR}/libogg"
        "${THIRDPARTY_BINARY_DIR}/libvorbis"
        "${THIRDPARTY_BINARY_DIR}/libtheora"
        "${THIRDPARTY_BINARY_DIR}/GTest"
    )

    # Configure path to cmake config files

    EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" 
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DBUILD_TESTS=${BUILD_TESTS}
            -DNEED_SDL2=${NEED_SDL2}
            -DNEED_OPENAL=${NEED_OPENAL}
            -DNEED_ASSIMP=${NEED_ASSIMP}
            -DNEED_OGG=${NEED_OGG}
            "${THIRDPARTY_SOURCE_DIR}"
        WORKING_DIRECTORY "${THIRDPARTY_BINARY_DIR}"
        RESULT_VARIABLE result
    )
    IF(result)
        MESSAGE("${result}")
        MESSAGE(FATAL_ERROR "Failed to configure ThirdParty projects")
    ENDIF()

    SET(BUILD_COMMAND ${CMAKE_COMMAND} --build . )
    IF("${CMAKE_GENERATOR}" STREQUAL "Unix Makefiles")
        SET(BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} -l )
    ENDIF()

    EXECUTE_PROCESS(
        COMMAND ${BUILD_COMMAND}
        WORKING_DIRECTORY "${THIRDPARTY_BINARY_DIR}"
        RESULT_VARIABLE result
    )

    IF(result)
        MESSAGE("${result}")
        MESSAGE(FATAL_ERROR "Failed to build ThirdParty projects")
    ENDIF()
ENDIF()

FIND_PACKAGE(glm CONFIG REQUIRED)
FIND_PACKAGE(nlohmann_json CONFIG REQUIRED)
FIND_PACKAGE(Python3 3.6 REQUIRED COMPONENTS Development)
FIND_PACKAGE(Threads REQUIRED)

IF(NEED_SDL2)
    FIND_PACKAGE(SDL2 2.0.6 CONFIG REQUIRED)
ELSE()
    FIND_PACKAGE(SDL2 2.0.6 CONFIG QUIET)
ENDIF()

IF(NEED_OPENAL)
    FIND_PACKAGE(OpenAL CONFIG REQUIRED)
ELSE()
    FIND_PACKAGE(OpenAL CONFIG QUIET)
ENDIF()

IF(NEED_ASSIMP)
    FIND_PACKAGE(assimp CONFIG 5.0 REQUIRED)
ELSE()
    FIND_PACKAGE(assimp CONFIG 5.0 QUIET)
ENDIF()

IF(NEED_OGG)
    FIND_PACKAGE(Ogg COMPONENTS Vorbis Theora REQUIRED)
ELSE()
    FIND_PACKAGE(Ogg COMPONENTS Vorbis Theora QUIET)
ENDIF()


IF(BUILD_TESTS)
    FIND_PACKAGE(GTest REQUIRED)
ENDIF()

IF(TARGET assimp::assimp)
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
ENDIF()

# Allow custom targets in Visual Studio to find .dll files
SET(RUNTIME_SEARCH_PATH
    ${SDL2_RUNTIME_DIR}
    ${OpenAL_RUNTIME_DIR}
)

###
### Modules
###

ADD_SUBDIRECTORY(Modules)

###
### Demos
###

# Allow custom targets to find .dlls
SET(CMAKE_MSVC_RUN_PATH "${RUNTIME_SEARCH_PATH}")

IF(BUILD_DEMOS)
    ADD_SUBDIRECTORY(Demos)
ENDIF()

###
### Packaging
###

IF(BUILD_PACKAGES)
    FIND_PROGRAM(
        TAR_EXECUTABLE
        tar
    )

    IF(TAR_EXECUTABLE)
        SET(_PACKAGE "libdusk-${PROJECT_VERSION}")

        FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${_PACKAGE}")

        ADD_CUSTOM_COMMAND(
            OUTPUT "${_PACKAGE}.tgz"
            DEPENDS ${_ENGINE}
            COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX="" ${CMAKE_SOURCE_DIR}
            COMMAND ${CMAKE_COMMAND} -E env
                DESTDIR="${CMAKE_BINARY_DIR}/${_PACKAGE}"
                ${CMAKE_COMMAND} --build . --target install
            COMMAND ${TAR_EXECUTABLE} -cvzf "${CMAKE_BINARY_DIR}/${_PACKAGE}.tgz" "${CMAKE_BINARY_DIR}/${_PACKAGE}/*"
        )

        ADD_CUSTOM_TARGET(
            tgz
            DEPENDS "${_PACKAGE}.tgz"
        )
    ENDIF()

    FIND_PROGRAM(
        DPKG_DEB_EXECUTABLE
        dpkg-deb
    )

    IF(DPKG_DEB_EXECUTABLE)
        SET(_PACKAGE "libdusk-dev_${PROJECT_VERSION}-1")

        FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${_PACKAGE}/DEBIAN")

        CONFIGURE_FILE(
            "${CMAKE_SOURCE_DIR}/CMake/DEBIAN/control.in"
            "${CMAKE_BINARY_DIR}/${_PACKAGE}/DEBIAN/control"
            @ONLY
        )

        ADD_CUSTOM_COMMAND(
            OUTPUT "${_PACKAGE}.deb"
            DEPENDS ${_ENGINE}
            COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX="/usr/local" ${CMAKE_SOURCE_DIR}
            COMMAND ${CMAKE_COMMAND} -E env
                DESTDIR="${CMAKE_BINARY_DIR}/${_PACKAGE}"
                ${CMAKE_COMMAND} --build . --target install
            COMMAND ${DPKG_DEB_EXECUTABLE} --build "${CMAKE_BINARY_DIR}/${_PACKAGE}"
        )

        ADD_CUSTOM_TARGET(
            deb
            DEPENDS "${_PACKAGE}.deb"
        )
    ENDIF()
ENDIF()

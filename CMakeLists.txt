CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)
CMAKE_POLICY(SET CMP0048 NEW)
IF(NOT ${CMAKE_VERSION} VERSION_LESS "3.13")
    CMAKE_POLICY(SET CMP0076 NEW)
ENDIF()

PROJECT(Dusk)

###
### Options
###

OPTION(VERBOSE_LOGGING "Build Dusk with Verbose Logging enabled" OFF)
IF(VERBOSE_LOGGING)
    ADD_DEFINITIONS(-DDUSK_ENABLE_VERBOSE_LOGGING)
ENDIF()

OPTION(BINARY_SHADERS "Build Dusk with Binary Shader support" OFF)
IF(BINARY_SHADERS)
    ADD_DEFINITIONS(-DDUSK_ENABLE_BINARY_SHADERS)
ENDIF()

OPTION(BUILD_DEMOS "Build Dusk Demo Projects" ON)

OPTION(BUILD_THIRDPARTY "Build Dusk ThirdParty Dependencies" ON)

###
### Configuration
###

# Allow for custom CMake modules
LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/CMake")

# Allow for custom organization of files in VisualStudio
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Get Git revision
INCLUDE(GetGitRevisionDescription)
GET_GIT_HEAD_REVISION(GIT_REFSPEC PROJECT_VERSION_GIT_HASH)
STRING(SUBSTRING "${PROJECT_VERSION_GIT_HASH}" 0 8 PROJECT_VERSION_GIT_HASH)

GIT_DESCRIBE(VERSION --tags)
STRING(REGEX REPLACE "^v([0-9]+)\\..*" "\\1" PROJECT_VERSION_MAJOR "${VERSION}")
STRING(REGEX REPLACE "^v[0-9]+\\.([0-9]+).*" "\\1" PROJECT_VERSION_MINOR "${VERSION}")
STRING(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" PROJECT_VERSION_PATCH "${VERSION}")
SET(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# Minimum OpenGL Version
SET(MIN_OPENGL_VERSION "4.1" CACHE STRING "Minimum supported OpenGL Version")

STRING(REPLACE "." ";" _VERSION_LIST ${MIN_OPENGL_VERSION})
LIST(GET _VERSION_LIST 0 _MIN_OPENGL_VERSION_MAJOR)
LIST(GET _VERSION_LIST 1 _MIN_OPENGL_VERSION_MINOR)

ADD_DEFINITIONS(-DDUSK_MIN_OPENGL_VERSION_MAJOR=${_MIN_OPENGL_VERSION_MAJOR}
                -DDUSK_MIN_OPENGL_VERSION_MINOR=${_MIN_OPENGL_VERSION_MINOR})

SET(_ASSET_PATH "${CMAKE_SOURCE_DIR}/Assets")

IF(MSVC)
    STRING(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
ENDIF()

###
### Third Party Dependencies
###

IF(BUILD_THIRDPARTY)
    FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/ThirdParty")

    SET(GLM_ROOT_DIR    "${CMAKE_BINARY_DIR}/ThirdParty/glm")
    SET(JSON_ROOT_DIR   "${CMAKE_BINARY_DIR}/ThirdParty/json")
    SET(SDL2_ROOT_DIR   "${CMAKE_BINARY_DIR}/ThirdParty/SDL2")
    SET(OpenAL_ROOT_DIR "${CMAKE_BINARY_DIR}/ThirdParty/OpenAL")
    SET(Ogg_ROOT_DIR    "${CMAKE_BINARY_DIR}/ThirdParty/libogg")
    SET(Vorbis_ROOT_DIR "${CMAKE_BINARY_DIR}/ThirdParty/libvorbis")
    SET(Theora_ROOT_DIR "${CMAKE_BINARY_DIR}/ThirdParty/libtheora")

    EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" "${CMAKE_SOURCE_DIR}/ThirdParty"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/ThirdParty"
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
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/ThirdParty"
        RESULT_VARIABLE result
    )

    IF(result)
        MESSAGE("${result}")
        MESSAGE(FATAL_ERROR "Failed to build ThirdParty projects")
    ENDIF()
ENDIF()

FIND_PACKAGE(GLM REQUIRED)
FIND_PACKAGE(JSON REQUIRED)
FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(OpenAL REQUIRED)
FIND_PACKAGE(Ogg REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

# Allow VS to find .dll's
SET(CMAKE_MSVCIDE_RUN_PATH 
    ${SDL2_RUNTIME_DIR}
    ${OpenAL_RUNTIME_DIR}
)

###
### Projects
###

ADD_SUBDIRECTORY(Engine)

###
### Demos
###

IF(BUILD_DEMOS)    
    ADD_SUBDIRECTORY(Demos/HelloWorld)
    ADD_SUBDIRECTORY(Demos/3DSound)
    ADD_SUBDIRECTORY(Demos/Snake)
ENDIF()

###
### Packaging
###

FIND_PROGRAM(
    TAR_EXECUTABLE
    tgz
)

IF (TAR_EXECUTABLE)
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

IF (DPKG_DEB_EXECUTABLE)
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

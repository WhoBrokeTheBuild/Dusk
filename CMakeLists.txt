CMAKE_MINIMUM_REQUIRED(VERSION 3.15 FATAL_ERROR)

# Allow for custom CMake modules
LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/CMake")

INCLUDE(Policies)

###
### Project
###

PROJECT(Dusk)

INCLUDE(Version)

###
### Options
###

OPTION(VERBOSE_LOGGING  "Build Dusk with Verbose Logging enabled"            OFF)
IF(VERBOSE_LOGGING)
    ADD_DEFINITIONS(-DDUSK_ENABLE_VERBOSE_LOGGING)
ENDIF()

OPTION(BINARY_SHADERS   "Build Dusk with Binary Shader support"               OFF)
IF(BINARY_SHADERS)
    ADD_DEFINITIONS(-DDUSK_ENABLE_BINARY_SHADERS)
ENDIF()

OPTION(BUILD_TESTS      "Build Tests"                                       OFF)
IF(BUILD_TESTS)
    ENABLE_TESTING()
ENDIF()

OPTION(BUILD_DEMOS      "Build Demo Projects"                               ON)
OPTION(BUILD_THIRDPARTY "Build Missing ThirdParty Dependencies"             ON)
OPTION(BUILD_PACKAGES   "Build Packages"                                    OFF)

SET(MODULES
    "All"
    CACHE STRING
    "Semicolon-separated list of required modules, use 'All' to build all modules"
)

###
### Modules Configuration
###

# Generate list of available modules
SET(_available_modules
    "Archive"
    "FreeType"
    "GLTF2"
    "OpenAL"
    "OpenGL"
    "STBI"
    "TinyOBJ"
    "Vulkan"
)
IF(WIN32)
    LIST(APPEND _available_modules "DirectX")
ENDIF()

IF(BUILD_DEMOS OR MODULES STREQUAL "All")
    SET(MODULES "${_available_modules}")
ENDIF()

# Filter requested modules that are not available
SET(_modules "")
FOREACH(_module ${MODULES})
    IF(${_module} IN_LIST _available_modules)
        LIST(APPEND _modules ${_module})
    ELSE()
        MESSAGE(WARNING "Module '${_module}' not supported on this platform")
ENDIF()
ENDFOREACH()

# Generate list of required third party dependencies
SET(_dependencies
    "glm"
    "GSL"
    "nlohmann_json"
    "Python3"
)

IF("OpenGL" IN_LIST _modules)
    LIST(APPEND _dependencies "SDL2")
ENDIF()

IF("Vulkan" IN_LIST _modules)
    LIST(APPEND _dependencies "SDL2" "VulkanSDK")
ENDIF()

IF("OpenAL" IN_LIST _modules)
    LIST(APPEND _dependencies "OpenAL")
ENDIF()

IF("FreeType" IN_LIST _modules)
    LIST(APPEND _dependencies "freetype")
ENDIF()

IF("Archive" IN_LIST _modules)
ENDIF()

IF(BUILD_TESTS)
    LIST(APPEND _dependencies "GTest")
ENDIF()

LIST(REMOVE_DUPLICATES _modules)
LIST(REMOVE_DUPLICATES _dependencies)

MESSAGE(STATUS "Available modules: ${_available_modules}")
MESSAGE(STATUS "Building Modules: ${_modules}")
MESSAGE(STATUS "Required Dependencies: ${_dependencies}")

###
### Configuration
###

INCLUDE(Compiler)

# Allow for custom organization of files in VisualStudio
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Don't use the install RPATH when building
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

###
### Third Party Dependencies
###

IF(BUILD_THIRDPARTY)
    SET(THIRDPARTY_SOURCE_DIR ${CMAKE_SOURCE_DIR}/ThirdParty)
    SET(THIRDPARTY_BINARY_DIR ${CMAKE_BINARY_DIR}/ThirdParty)

    FILE(MAKE_DIRECTORY ${THIRDPARTY_BINARY_DIR})

    # Set _ROOT variables for config-based packages
    SET(glm_ROOT            ${THIRDPARTY_BINARY_DIR}/glm/cmake)
    SET(nlohmann_json_ROOT  ${THIRDPARTY_BINARY_DIR}/nlohmann_json)
    SET(SDL2_ROOT           ${THIRDPARTY_BINARY_DIR}/SDL2)
    SET(OpenAL_ROOT         ${THIRDPARTY_BINARY_DIR}/OpenAL)
    SET(freetype_ROOT       ${THIRDPARTY_BINARY_DIR}/freetype)

    # Set CMAKE_PREFIX_PATH for find-based packages
    LIST(APPEND CMAKE_PREFIX_PATH
        ${THIRDPARTY_BINARY_DIR}/GSL
        ${THIRDPARTY_BINARY_DIR}/Python3
        ${THIRDPARTY_BINARY_DIR}/libogg
        ${THIRDPARTY_BINARY_DIR}/libvorbis
        ${THIRDPARTY_BINARY_DIR}/libtheora
        ${THIRDPARTY_BINARY_DIR}/GTest
    )

    STRING(REPLACE ";" ":" _tmp "${_dependencies}")

    EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" 
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DDEPENDENCIES=${_tmp}
            ${THIRDPARTY_SOURCE_DIR}
        WORKING_DIRECTORY ${THIRDPARTY_BINARY_DIR}
        RESULT_VARIABLE _result
    )
    IF(_result)
        MESSAGE("${_result}")
        MESSAGE(FATAL_ERROR "Failed to configure ThirdParty projects")
    ENDIF()

    SET(_THIRDPARTY_BUILD_COMMAND ${CMAKE_COMMAND} --build . )
    IF(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
        SET(_THIRDPARTY_BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} -l )
    ENDIF()

    EXECUTE_PROCESS(
        COMMAND ${_THIRDPARTY_BUILD_COMMAND}
        WORKING_DIRECTORY ${THIRDPARTY_BINARY_DIR}
        RESULT_VARIABLE _result
    )

    IF(_result)
        MESSAGE("${_result}")
        MESSAGE(FATAL_ERROR "Failed to build ThirdParty projects")
    ENDIF()
ENDIF()

IF("glm" IN_LIST _dependencies)
FIND_PACKAGE(glm CONFIG REQUIRED)

IF(NOT TARGET glm::glm)
    MESSAGE(SEND_ERROR "Incompatible version of glm found")
ENDIF()
ENDIF()

IF("GSL" IN_LIST _dependencies)
FIND_PACKAGE(GSL REQUIRED)
ENDIF()

IF("nlohmann_json" IN_LIST _dependencies)
FIND_PACKAGE(nlohmann_json CONFIG REQUIRED)
ENDIF()

IF("Python3" IN_LIST _dependencies)
    FIND_PACKAGE(Python3 3.8 REQUIRED
        COMPONENTS Development)
ENDIF()

IF("SDL2" IN_LIST _dependencies)
    FIND_PACKAGE(SDL2 2.0.6 CONFIG REQUIRED)
ENDIF()

IF("VulkanSDK" IN_LIST _dependencies)
    FIND_PACKAGE(VulkanSDK REQUIRED)
ENDIF()

IF("OpenAL" IN_LIST _dependencies)
    FIND_PACKAGE(OpenAL CONFIG REQUIRED)
ENDIF()

IF("freetype" IN_LIST _dependencies)
    FIND_PACKAGE(freetype CONFIG REQUIRED)
ENDIF()

IF("Ogg" IN_LIST _dependencies)
    FIND_PACKAGE(Ogg REQUIRED 
        COMPONENTS Vorbis Theora)
ENDIF()

IF("GTest" IN_LIST _dependencies)
    FIND_PACKAGE(GTest REQUIRED)
ENDIF()

# Compile list of runtime directories

IF(TARGET SDL2::SDL2)
    GET_TARGET_PROPERTY(_location SDL2::SDL2 IMPORTED_LOCATION)
    GET_FILENAME_COMPONENT(SDL2_RUNTIME_DIR ${_location} DIRECTORY)
    
    GET_TARGET_PROPERTY(_location SDL2::SDL2 IMPORTED_LOCATION_DEBUG)
    GET_FILENAME_COMPONENT(SDL2_RUNTIME_DIR_DEBUG ${_location} DIRECTORY)
    
    GET_TARGET_PROPERTY(_location SDL2::SDL2 IMPORTED_LOCATION_RELEASE)
    GET_FILENAME_COMPONENT(SDL2_RUNTIME_DIR_RELEASE ${_location} DIRECTORY)
ENDIF()

IF(TARGET OpenAL::OpenAL)
    GET_TARGET_PROPERTY(_location OpenAL::OpenAL IMPORTED_LOCATION)
    GET_FILENAME_COMPONENT(OpenAL_RUNTIME_DIR ${_location} DIRECTORY)

    GET_TARGET_PROPERTY(_location OpenAL::OpenAL IMPORTED_LOCATION_DEBUG)
    GET_FILENAME_COMPONENT(OpenAL_RUNTIME_DIR_DEBUG ${_location} DIRECTORY)

    GET_TARGET_PROPERTY(_location OpenAL::OpenAL IMPORTED_LOCATION_RELEASE)
    GET_FILENAME_COMPONENT(OpenAL_RUNTIME_DIR_RELEASE ${_location} DIRECTORY)
ENDIF()

# Allow custom targets in Visual Studio to find .dll files
SET(RUNTIME_SEARCH_PATH
    ${SDL2_RUNTIME_DIR}
    ${OpenAL_RUNTIME_DIR}
    ${Python3_RUNTIME_DIR}
    ${Ogg_RUNTIME_DIR}
    ${Vorbis_RUNTIME_DIR}
    ${Theora_RUNTIME_DIR}
)

SET(RUNTIME_SEARCH_PATH_DEBUG
    ${SDL2_RUNTIME_DIR_DEBUG}
    ${OpenAL_RUNTIME_DIR_DEBUG}
    ${assimp_RUNTIME_DIR_DEBUG}
)

SET(RUNTIME_SEARCH_PATH_RELEASE
    ${SDL2_RUNTIME_DIR_RELEASE}
    ${OpenAL_RUNTIME_DIR_RELEASE}
    ${assimp_RUNTIME_DIR_RELEASE}
)

###
### Engine
###

ADD_SUBDIRECTORY(Engine)

###
### Modules
###

ADD_SUBDIRECTORY(Modules)

###
### Demos
###

# Allow custom targets to find .dlls
SET(CMAKE_MSVCIDE_RUN_PATH ${RUNTIME_SEARCH_PATH})

IF(BUILD_DEMOS)
    ADD_SUBDIRECTORY(Demos)
ENDIF()


###
### Install Configuration
###

INSTALL(
    TARGETS ${MODULE_TARGETS}
    EXPORT DuskTargets
    INCLUDES DESTINATION include
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

INCLUDE(CMakePackageConfigHelpers)

WRITE_BASIC_PACKAGE_VERSION_FILE(
    "${CMAKE_BINARY_DIR}/DuskConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

INSTALL(EXPORT DuskTargets
    FILE DuskTargets.cmake
    NAMESPACE Dusk::
    DESTINATION lib/cmake/Dusk
)

CONFIGURE_PACKAGE_CONFIG_FILE(
    "${CMAKE_SOURCE_DIR}/CMake/DuskConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/DuskConfig.cmake"
    INSTALL_DESTINATION lib/cmake/Dusk
)

INSTALL(EXPORT DuskTargets 
    DESTINATION lib/cmake/Dusk
)

INSTALL(FILES 
    "${CMAKE_BINARY_DIR}/DuskConfigVersion.cmake" 
    "${CMAKE_BINARY_DIR}/DuskConfig.cmake"
    DESTINATION lib/cmake/Dusk
)

FIND_PACKAGE(PkgConfig QUIET)

IF(PKG_CONFIG_FOUND)
    CONFIGURE_FILE(
        "${CMAKE_SOURCE_DIR}/CMake/Dusk.pc.in"
        "${CMAKE_BINARY_DIR}/Dusk.pc"
        @ONLY
    )

    INSTALL(FILES
        "${CMAKE_BINARY_DIR}/Dusk.pc"
        DESTINATION share/pkgconfig
    )
ENDIF()

###
### Packaging
###

IF(BUILD_PACKAGES)
    ADD_SUBDIRECTORY(Packages)
ENDIF()

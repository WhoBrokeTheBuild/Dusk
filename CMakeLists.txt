CMAKE_MINIMUM_REQUIRED(VERSION 3.13 FATAL_ERROR)

# Allow for custom CMake modules
LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/CMake")

INCLUDE(Policies)

###
### Project
###

PROJECT(Dusk)

###
### Options
###

OPTION(VERBOSE_LOGGING "Build Dusk with Verbose Logging enabled"            OFF)
IF(VERBOSE_LOGGING)
    ADD_DEFINITIONS(-DDUSK_ENABLE_VERBOSE_LOGGING)
ENDIF()

OPTION(BINARY_SHADERS "Build Dusk with Binary Shader support"               OFF)
IF(BINARY_SHADERS)
    ADD_DEFINITIONS(-DDUSK_ENABLE_BINARY_SHADERS)
ENDIF()

OPTION(BUILD_TESTS      "Build Tests"                                       OFF)
IF(BUILD_TESTS)
    ENABLE_TESTING()
ENDIF()

OPTION(BUILD_DEMOS      "Build Demo Projects"                               ON)
OPTION(BUILD_ThirdParty "Build Missing ThirdParty Dependencies"             ON)
OPTION(BUILD_PACKAGES   "Build Packages"                                    OFF)

# Optional Modules

OPTION(NEED_MODULE_OpenGL      "Require the OpenGL Module"                  ON)
OPTION(NEED_MODULE_DirectX     "Require the DirectX Module (Windows Only)"  OFF)
OPTION(NEED_MODULE_Vulkan      "Require the Vulkan Module"                  OFF)
OPTION(NEED_MODULE_OpenAL      "Require the OpenAL Module"                  OFF)
OPTION(NEED_MODULE_Assimp      "Require the Assimp Module"                  OFF)
OPTION(NEED_MODULE_freetype    "Require the freetype Module"                ON)
OPTION(NEED_MODULE_Archive     "Require the Archive Module"                 OFF)

IF(NEED_MODULE_OpenGL OR NEED_MODULE_Vulkan)
    SET(NEED_SDL2 TRUE)
ENDIF()

IF(NEED_MODULE_Vulkan)
    SET(NEED_VulkanSDK TRUE)
ENDIF()

IF(NEED_MODULE_OpenAL)
    SET(NEED_OpenAL TRUE)
ENDIF()

IF(NEED_MODULE_Assimp)
    SET(NEED_assimp TRUE)
ENDIF()

IF(NEED_MODULE_freetype)
    SET(NEED_freetype TRUE)
ENDIF()

IF(NEED_MODULE_Archive)
ENDIF()

IF(BUILD_TESTS)
    SET(NEED_GTest TRUE)
ENDIF()

###
### Configuration
###

INCLUDE(Compiler)
INCLUDE(Version)

# Allow for custom organization of files in VisualStudio
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Don't use the install RPATH when building
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

###
### Third Party Dependencies
###

IF(BUILD_ThirdParty)
    SET(ThirdParty_SOURCE_DIR ${CMAKE_SOURCE_DIR}/ThirdParty)
    SET(ThirdParty_BINARY_DIR ${CMAKE_BINARY_DIR}/ThirdParty)

    FILE(MAKE_DIRECTORY ${ThirdParty_BINARY_DIR})

    # Set _ROOT variables for config-based packages
    SET(glm_ROOT            ${ThirdParty_BINARY_DIR}/glm)
    SET(nlohmann_json_ROOT  ${ThirdParty_BINARY_DIR}/nlohmann_json)
    SET(SDL2_ROOT           ${ThirdParty_BINARY_DIR}/SDL2)
    SET(OpenAL_ROOT         ${ThirdParty_BINARY_DIR}/OpenAL)
    SET(assimp_ROOT         ${ThirdParty_BINARY_DIR}/assimp)
    SET(freetype_ROOT       ${ThirdParty_BINARY_DIR}/freetype)

    # Set CMAKE_PREFIX_PATH for find-based packages
    LIST(APPEND CMAKE_PREFIX_PATH
        ${ThirdParty_BINARY_DIR}/GSL
        ${ThirdParty_BINARY_DIR}/Python3
        ${ThirdParty_BINARY_DIR}/libogg
        ${ThirdParty_BINARY_DIR}/libvorbis
        ${ThirdParty_BINARY_DIR}/libtheora
        ${ThirdParty_BINARY_DIR}/GTest
    )

    # Configure path to cmake config files

    EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" 
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DNEED_SDL2=${NEED_SDL2}
            -DNEED_OpenAL=${NEED_OpenAL}
            -DNEED_assimp=${NEED_assimp}
            -DNEED_freetype=${NEED_freetype}
            -DNEED_GTest=${NEED_GTest}
            ${ThirdParty_SOURCE_DIR}
        WORKING_DIRECTORY ${ThirdParty_BINARY_DIR}
        RESULT_VARIABLE _result
    )
    IF(_result)
        MESSAGE("${_result}")
        MESSAGE(FATAL_ERROR "Failed to configure ThirdParty projects")
    ENDIF()

    SET(_ThirdParty_BUILD_COMMAND ${CMAKE_COMMAND} --build . )
    IF(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
        SET(_ThirdParty_BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} -l )
    ENDIF()

    EXECUTE_PROCESS(
        COMMAND ${_ThirdParty_BUILD_COMMAND}
        WORKING_DIRECTORY ${ThirdParty_BINARY_DIR}
        RESULT_VARIABLE _result
    )

    IF(_result)
        MESSAGE("${_result}")
        MESSAGE(FATAL_ERROR "Failed to build ThirdParty projects")
    ENDIF()
ENDIF()

FIND_PACKAGE(glm CONFIG REQUIRED)
FIND_PACKAGE(GSL REQUIRED)
FIND_PACKAGE(nlohmann_json CONFIG REQUIRED)
FIND_PACKAGE(Python3 3.6 COMPONENTS Development REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

FIND_PACKAGE(SDL2 2.0.6 CONFIG)
IF(NEED_SDL2 AND NOT SDL2_FOUND)
    MESSAGE(SEND_ERROR "NEED_SDL2 is set, but SDL2 could not be found.")
ENDIF()

FIND_PACKAGE(VulkanSDK)
IF(NEED_VulkanSDK AND NOT VulkanSDK_FOUND)
    MESSAGE(SEND_ERROR "NEED_VulkanSDK is set, but the Vulkan SDK could not be found.")
ENDIF()

FIND_PACKAGE(OpenAL CONFIG)
IF(NEED_OpenAL AND NOT OpenAL_FOUND)
    MESSAGE(SEND_ERROR "NEED_OpenAL is set, but OpenAL could not be found.")
ENDIF()

FIND_PACKAGE(assimp 5.0 CONFIG)
IF(NEED_assimp AND NOT assimp_FOUND)
    MESSAGE(SEND_ERROR "NEED_assimp is set, but assimp could not be found.")
ENDIF()

FIND_PACKAGE(freetype CONFIG)
IF(NEED_freetype AND NOT freetype_FOUND)
    MESSAGE(SEND_ERROR "NEED_freetype is set, but freetype could not be found.")
ENDIF()

FIND_PACKAGE(Ogg COMPONENTS Vorbis Theora)
IF(NEED_Ogg AND NOT Ogg_FOUND)
    MESSAGE(SEND_ERROR "NEED_Ogg is set, but Ogg could not be found.")
ENDIF()

IF(NEED_GTest)
    FIND_PACKAGE(GTest REQUIRED)
ENDIF()

IF(TARGET SDL2::SDL2)
    GET_TARGET_PROPERTY(_location SDL2::SDL2 IMPORTED_LOCATION_RELEASE)
    GET_FILENAME_COMPONENT(SDL2_RUNTIME_DIR ${_location} DIRECTORY)
ENDIF()

IF(TARGET OpenAL::OpenAL)
    GET_TARGET_PROPERTY(_location OpenAL::OpenAL IMPORTED_LOCATION_RELEASE)
    GET_FILENAME_COMPONENT(OpenAL_RUNTIME_DIR ${_location} DIRECTORY)
ENDIF()

IF(TARGET assimp::assimp)
    # Extract release-specific configuration
    GET_TARGET_PROPERTY(soname   assimp::assimp IMPORTED_SONAME_RELEASE)
    GET_TARGET_PROPERTY(implib   assimp::assimp IMPORTED_IMPLIB_RELEASE)
    GET_TARGET_PROPERTY(_location assimp::assimp IMPORTED_LOCATION_RELEASE)

    GET_FILENAME_COMPONENT(assimp_RUNTIME_DIR ${_location} DIRECTORY)

    # Set build type agnostic configuration for assimp
    SET_TARGET_PROPERTIES(
        assimp::assimp 
        PROPERTIES
            IMPORTED_SONAME         ${_soname}
            IMPORTED_IMPLIB         ${_implib}
            IMPORTED_LOCATION       ${_location}
            IMPORTED_SONAME_DEBUG   ${_soname}
            IMPORTED_IMPLIB_DEBUG   ${_implib}
            IMPORTED_LOCATION_DEBUG ${_location}
    )
ENDIF()

# Allow custom targets in Visual Studio to find .dll files
SET(RUNTIME_SEARCH_PATH
    ${SDL2_RUNTIME_DIR}
    ${OpenAL_RUNTIME_DIR}
    ${Python3_RUNTIME_DIR}
    ${Ogg_RUNTIME_DIR}
    ${Vorbis_RUNTIME_DIR}
    ${Theora_RUNTIME_DIR}
    ${assimp_RUNTIME_DIR}
)

###
### Modules
###

INCLUDE(ShaderDataBindings)
INCLUDE(ShaderInputLocations)

ADD_SHADER_DATA_BINDING(TRANSFORM)
ADD_SHADER_DATA_BINDING(MATERIAL)

ADD_SHADER_INPUT_LOCATION(POSITION)
ADD_SHADER_INPUT_LOCATION(NORMAL)
ADD_SHADER_INPUT_LOCATION(TANGENT)
ADD_SHADER_INPUT_LOCATION(BITANGENT)
ADD_SHADER_INPUT_LOCATION(COLOR)
ADD_SHADER_INPUT_LOCATION(UV)

ADD_SUBDIRECTORY(Modules)

###
### Demos
###

# Allow custom targets to find .dlls
SET(CMAKE_MSVCIDE_RUN_PATH ${RUNTIME_SEARCH_PATH})

IF(BUILD_DEMOS)
    ADD_SUBDIRECTORY(Demos)
ENDIF()


GET_SHADER_DATA_BINDINGS(bindings)
GET_SHADER_INPUT_LOCATIONS(locations)

SET(binding_defines "")
SET(location_defines "")

FOREACH(binding ${bindings})
    SET(bindings_include_content "${bindings_include_content}\n#define ${binding}")
ENDFOREACH()

FOREACH(location ${locations})
    SET(locations_include_content "${locations_include_content}\n#define ${location}")
ENDFOREACH()

FILE(WRITE "${CMAKE_BINARY_DIR}/shader_data_bindings.glsl" ${bindings_include_content})
FILE(WRITE "${CMAKE_BINARY_DIR}/uniform_locations.glsl" ${locations_include_content})

###
### Packaging
###

IF(BUILD_PACKAGES)
    ADD_SUBDIRECTORY(Packages)
ENDIF()

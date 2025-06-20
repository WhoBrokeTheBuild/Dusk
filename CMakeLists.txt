cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

# Allow for custom CMake modules
list(PREPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

project(Dusk)

include(DuskProjectVersion)

###
### Options
###

option(
    DUSK_TEST
    "Build Dusk test suite"
    ON
)

option(
    DUSK_BENCHMARK
    "Build Dusk with benchmarking"
    OFF
)

set(
    GLTF_SAMPLE_ASSETS_PATH "${GLTF_SAMPLE_ASSETS_PATH}"
    CACHE STRING
    "Path to the glTF-Sample-Assets repo"
)

if(NOT CMAKE_BUILD_TYPE)

    list(JOIN "${CMAKE_CONFIGURATION_TYPES}" ", " _config_types)

    set(CMAKE_BUILD_TYPE "Release"
        CACHE STRING
        "Type of build to configure, can be one of: ${_config_types} [default: Release]"
        FORCE
    )

endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

if(DUSK_TEST)
    include(CTest)
endif()

###
### Third Party Dependencies
###

find_package(Vulkan COMPONENTS glslc REQUIRED)

include(FetchContent)
set(_fetch_content_list "")

# TODO: Update once this is included in the next version
# https://github.com/g-truc/glm/commit/a2844eede81f92b7dfb327f831c0bc0dbb273078
FetchContent_Declare(
    glm
    URL "https://github.com/g-truc/glm/archive/refs/heads/master.zip"
    FIND_PACKAGE_ARGS 1.0.1
)
list(APPEND _fetch_content_list glm)

FetchContent_Declare(
    nlohmann_json
    URL "https://github.com/nlohmann/json/archive/refs/tags/v3.12.0.zip"
    FIND_PACKAGE_ARGS 3.12.0
)
list(APPEND _fetch_content_list nlohmann_json)

FetchContent_Declare(
    SDL3
    URL "https://github.com/libsdl-org/SDL/archive/refs/tags/release-3.2.16.zip"
    FIND_PACKAGE_ARGS 3.2.16
)
list(APPEND _fetch_content_list SDL3)

set(SPIRV_REFLECT_EXECUTABLE OFF CACHE INTERNAL "")
set(SPIRV_REFLECT_STATIC_LIB ON  CACHE INTERNAL "")

FetchContent_Declare(
    spirv-reflect
    URL "https://github.com/KhronosGroup/SPIRV-Reflect/archive/refs/tags/vulkan-sdk-1.3.290.0.zip"
)
list(APPEND _fetch_content_list spirv-reflect)

if(DUSK_TEST)
    FetchContent_Declare(
        googletest
        URL "https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip"
        FIND_PACKAGE_ARGS 1.17.0
    )

    list(APPEND _fetch_content_list googletest)
endif()

FetchContent_MakeAvailable(${_fetch_content_list})

###
### Globals
###

# Search path for all Assets (Shaders, Models, etc.)
set(DUSK_ASSET_PATH "${GLTF_SAMPLE_ASSETS_PATH}")

###
### Engine Library
###

add_subdirectory(Engine)

###
### Demo Executables
###

add_subdirectory(Demos)
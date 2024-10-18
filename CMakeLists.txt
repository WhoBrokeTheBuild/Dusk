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

###
### Third Party Dependencies
###

find_package(Vulkan COMPONENTS glslc REQUIRED)

include(FetchContent)

# TODO: Update once this is included in the next version
# https://github.com/g-truc/glm/commit/a2844eede81f92b7dfb327f831c0bc0dbb273078
FetchContent_Declare(
    glm
    URL "https://github.com/g-truc/glm/archive/refs/heads/master.zip"
    FIND_PACKAGE_ARGS 1.0.1
)

FetchContent_Declare(
    googletest
    URL "https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip"
    FIND_PACKAGE_ARGS 1.15.2
)

FetchContent_Declare(
    nlohmann_json
    URL "https://github.com/nlohmann/json/archive/refs/tags/v3.11.3.zip"
    FIND_PACKAGE_ARGS 3.11.3
)

# TODO: Update once SDL3 is stable
FetchContent_Declare(
    SDL3
    URL "https://github.com/libsdl-org/SDL/archive/refs/heads/main.zip"
    FIND_PACKAGE_ARGS 3
)

set(SPIRV_REFLECT_EXECUTABLE OFF CACHE INTERNAL "")
set(SPIRV_REFLECT_STATIC_LIB ON  CACHE INTERNAL "")

FetchContent_Declare(
    spirv-reflect
    URL "https://github.com/KhronosGroup/SPIRV-Reflect/archive/refs/tags/vulkan-sdk-1.3.290.0.zip"
)

FetchContent_MakeAvailable(glm nlohmann_json SDL3 spirv-reflect)

if(DUSK_TEST)
    include(CTest)
    FetchContent_MakeAvailable(googletest)
endif()

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
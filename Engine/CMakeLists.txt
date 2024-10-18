
set(_target DuskEngine)

###
### Source Files
###

file(GLOB_RECURSE
    _source_list
    "Public/*.hpp"
    "Private/*.hpp"
    "Private/*.cpp"
)

set(_config_hpp_in ${CMAKE_CURRENT_SOURCE_DIR}/Private/Config.hpp.in)
set(_config_hpp    ${CMAKE_CURRENT_BINARY_DIR}/Private/Config.hpp)
list(APPEND _source_list ${_config_hpp})

configure_file(${_config_hpp_in} ${_config_hpp})

set(_stb_image_file ${CMAKE_CURRENT_BINARY_DIR}/Private/stb_image.h)
list(APPEND _source_list ${_stb_image_file})

if(NOT EXISTS ${_stb_image_file})
    file(DOWNLOAD
        "https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_image.h"
        ${_stb_image_file}
    )
endif()

###
### Asset Processing
###

list(APPEND DUSK_ASSET_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Assets")
list(APPEND DUSK_ASSET_PATH "${CMAKE_CURRENT_BINARY_DIR}/Assets")
set(DUSK_ASSET_PATH "${DUSK_ASSET_PATH}" PARENT_SCOPE)

###
### Target Configuration
###

add_library(
    ${_target} SHARED
    ${_source_list}
)

target_link_libraries(
    ${_target}
    PUBLIC
        glm::glm
        nlohmann_json::nlohmann_json
        SDL3::SDL3
        Vulkan::Vulkan
        spirv-reflect-static
)

target_include_directories(
    ${_target}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
        ${CMAKE_CURRENT_BINARY_DIR}/Public
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private
        ${CMAKE_CURRENT_BINARY_DIR}/Private
)

target_compile_definitions(
    ${_target}
    PUBLIC
        # Disable VisualStudio "not secure" warnings
        $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>

        $<$<BOOL:${DUSK_BENCHMARK}>:DUSK_BENCHMARK>
)

target_compile_options(
    ${_target}
    PUBLIC
        # Configure exception handling model
        $<$<CXX_COMPILER_ID:MSVC>: /EHs>

        # Force windows to use UTF-8
        $<$<CXX_COMPILER_ID:MSVC>: /utf-8>

        # Enable most warnings, disable unknown pragmas warning
        $<$<CXX_COMPILER_ID:GNU>:   -Wall -Wno-unknown-pragmas -Wno-changes-meaning -Wno-sign-compare>
        $<$<CXX_COMPILER_ID:Clang>: -Wall -Wno-unknown-pragmas># -Wno-nullability-completeness -Wno-self-assign-overloaded>
        $<$<CXX_COMPILER_ID:MSVC>:  /wd4068>
)

set_target_properties(
    ${_target}
    PROPERTIES
        DEFINE_SYMBOL "DUSK_EXPORT"
)

###
### Tests
###

if(DUSK_TEST)

    file(GLOB
        _test_source_list
        "Tests/Test*.cpp"
    )

    foreach(_test_source IN LISTS _test_source_list)

        cmake_path(GET _test_source STEM _test_name)

        add_executable(
            ${_test_name}
            ${_test_source}
        )

        target_link_libraries(
            ${_test_name}
            PRIVATE
                ${_target}
                gtest
        )

        add_test(
            NAME ${_test_name}
            COMMAND $<TARGET_FILE:${_test_name}>
        )

    endforeach()

endif()
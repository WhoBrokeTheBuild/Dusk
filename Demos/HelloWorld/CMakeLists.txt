
file(GLOB_RECURSE
    _source_list
    "Source/*.hpp"
    "Source/*.cpp"
)

file(GLOB_RECURSE
    _shader_input_list
    "Assets/*.glsl"
    "Assets/*.vert"
    "Assets/*.frag"
    # TODO:
)

include(DuskCompileShaderList)

list(APPEND DUSK_ASSET_PATH "${CMAKE_CURRENT_BINARY_DIR}/Assets")
list(APPEND DUSK_ASSET_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Assets")

dusk_compile_shader_list(
    "${DUSK_ASSET_PATH}"
    "${_shader_input_list}"
    _shader_output_list
)

add_executable(
    HelloWorld
    ${_source_list}
    ${_shader_output_list}
)

target_link_libraries(
    HelloWorld
    PRIVATE
        DuskEngine
)

string(REPLACE ";" ":" _asset_path "${DUSK_ASSET_PATH}")

target_compile_definitions(
    HelloWorld
    PRIVATE
        DUSK_ASSET_PATH="${_asset_path}"
)
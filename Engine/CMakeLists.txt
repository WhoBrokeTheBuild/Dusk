
INCLUDE(DefineModule)
INCLUDE(CompileShaders)
INCLUDE(SetSourceGroups)

SET(_target DuskEngine)

DEFINE_MODULE(${_target} ENGINE)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        glm::glm
        GSL::GSL
        nlohmann_json::nlohmann_json
        cflags::cflags
        Python3::Python
        ${CMAKE_DL_LIBS}
)

###
### Shader Processing
###

FILE(
    GLOB_RECURSE
    _shader_includes
    Assets/Shaders/*.inc.glsl
    Assets/Shaders/*.inc.hlsl
)

SET(DUSK_ENGINE_SHADER_INCLUDES ${_shader_includes} PARENT_SCOPE)

FILE(
    GLOB_RECURSE
    _shaders_in
    Assets/Shaders/*.glsl
    Assets/Shaders/*.hlsl
)

SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${_shaders_out}")

LIST(APPEND DUSK_ASSET_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets/
    ${CMAKE_CURRENT_BINARY_DIR}/Assets/
)

SET(DUSK_ASSET_PATH ${DUSK_ASSET_PATH} PARENT_SCOPE)

COMPILE_SHADERS("${_shader_includes}" "${_shaders_in}" _shaders_out)

TARGET_SOURCES(
    ${_target}
    PRIVATE
        ${_shaders_in}
        ${_shaders_out}
)

SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_shaders_in}")
SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${_shaders_out}")
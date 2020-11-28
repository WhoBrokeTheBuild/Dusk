
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
        Python3::Python
        ${CMAKE_DL_LIBS}
)

###
### Shader Processing
###

FILE(
    GLOB_RECURSE
    _shaders_out
    Assets/Shaders/*.glsl
    Assets/Shaders/*.hlsl
)

SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${_shaders_out}")

LIST(APPEND ASSET_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets/
    ${CMAKE_CURRENT_BINARY_DIR}/Assets/
)

SET(ASSET_PATH ${ASSET_PATH} PARENT_SCOPE)

COMPILE_SHADERS("${_shaders_out}" _shaders_out)

TARGET_SOURCES(
    ${_target}
    PRIVATE
        ${_shaders_out}
)




FILE(GLOB_RECURSE
    _shader_configs
    Assets/Shaders/*.json
)

FOREACH(_config ${_shader_configs})
    STRING(REPLACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        _config_out
        ${_config}
    )

    GET_FILENAME_COMPONENT(_workdir ${_config} DIRECTORY)
    GET_FILENAME_COMPONENT(_binary_dir ${_config_out} DIRECTORY)

    ADD_CUSTOM_COMMAND(
        OUTPUT ${_config_out}
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Scripts/compile-shaders.py ${_config} ${_binary_dir}
        DEPENDS ${_config}
        WORKING_DIRECTORY ${_workdir}
    )

    TARGET_SOURCES(
        ${_target}
        PRIVATE
            ${_config_out}
    )
    
    
ENDFOREACH()
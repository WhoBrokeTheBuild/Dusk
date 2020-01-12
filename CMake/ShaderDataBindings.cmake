
SET(_shader_data_binding_index 0)
SET(_shader_data_bindings "")

FUNCTION(ADD_SHADER_DATA_BINDING name)
    LIST(APPEND _shader_data_bindings "${name}_DATA_BINDING ${_shader_data_binding_index}")

    MATH(EXPR _shader_data_binding_index "${_shader_data_binding_index}+1")
    SET(_shader_data_binding_index ${_shader_data_binding_index} PARENT_SCOPE)
    SET(_shader_data_bindings ${_shader_data_bindings} PARENT_SCOPE)
ENDFUNCTION()

FUNCTION(GET_SHADER_DATA_BINDINGS out)
    SET(${out} "${_shader_data_bindings}" PARENT_SCOPE)
ENDFUNCTION()

FUNCTION(WRITE_SHADER_DATA_BINDING_FILES cppout glslout)
    SET(SHADER_DATA_BINDINGS_DEFINES "")

    FOREACH(location ${_shader_input_locations})
        SET(SHADER_DATA_BINDINGS_DEFINES "${SHADER_DATA_BINDINGS_DEFINES}\n#define ${location}")
    ENDFOREACH()

    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/CMake/ShaderDataBindings.hpp.in
        ${cppout}
        @ONLY
    )

    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/CMake/ShaderDataBindings.inc.glsl.in
        ${glslout}
        @ONLY
    )
ENDFUNCTION()
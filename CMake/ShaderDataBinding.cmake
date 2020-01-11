
SET_PROPERTY(GLOBAL PROPERTY _shader_data_index 0)
SET_PROPERTY(GLOBAL PROPERTY _shader_data_bindings "")

FUNCTION(ADD_SHADER_DATA_BINDING name)
    GET_PROPERTY(shader_data_index GLOBAL PROPERTY _shader_data_index)
    GET_PROPERTY(shader_data_bindings GLOBAL PROPERTY _shader_data_bindings)
    
    LIST(APPEND shader_data_bindings "${name}_BINDING=${shader_data_index}")

    MATH(EXPR shader_data_index "${shader_data_index}+1")
    SET_PROPERTY(GLOBAL PROPERTY _shader_data_index ${shader_data_index})
    SET_PROPERTY(GLOBAL PROPERTY _shader_data_bindings ${shader_data_bindings})
ENDFUNCTION()

FUNCTION(GET_SHADER_DATA_BINDINGS out)
    GET_PROPERTY(shader_data_bindings GLOBAL PROPERTY _shader_data_bindings)
    SET(${out} "${shader_data_bindings}" PARENT_SCOPE)
ENDFUNCTION()

SET_PROPERTY(GLOBAL PROPERTY shader_data_binding_index 0)
SET_PROPERTY(GLOBAL PROPERTY shader_data_bindings "")

FUNCTION(ADD_SHADER_DATA_BINDING name)
    GET_PROPERTY(index      GLOBAL PROPERTY shader_data_binding_index)
    GET_PROPERTY(bindings   GLOBAL PROPERTY shader_data_bindings)
    
    LIST(APPEND bindings "${name}_BINDING ${index}")

    MATH(EXPR index "${index}+1")
    SET_PROPERTY(GLOBAL PROPERTY shader_data_binding_index ${index})
    SET_PROPERTY(GLOBAL PROPERTY shader_data_bindings ${bindings})
ENDFUNCTION()

FUNCTION(GET_SHADER_DATA_BINDINGS out)
    GET_PROPERTY(shader_data_bindings GLOBAL PROPERTY shader_data_bindings)
    SET(${out} "${shader_data_bindings}" PARENT_SCOPE)
ENDFUNCTION()
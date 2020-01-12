
SET_PROPERTY(GLOBAL PROPERTY shader_input_location_index 0)
SET_PROPERTY(GLOBAL PROPERTY shader_input_locations "")

FUNCTION(ADD_SHADER_INPUT_LOCATION name)
    GET_PROPERTY(index      GLOBAL PROPERTY shader_input_location_index)
    GET_PROPERTY(locations  GLOBAL PROPERTY shader_input_locations)
    
    LIST(APPEND locations "${name}_LOCATION ${index}")

    MATH(EXPR index "${index}+1")
    SET_PROPERTY(GLOBAL PROPERTY shader_input_location_index ${index})
    SET_PROPERTY(GLOBAL PROPERTY shader_input_locations ${locations})
ENDFUNCTION()

FUNCTION(GET_SHADER_INPUT_LOCATIONS out)
    GET_PROPERTY(locations GLOBAL PROPERTY shader_input_locations)
    SET(${out} "${locations}" PARENT_SCOPE)
ENDFUNCTION()
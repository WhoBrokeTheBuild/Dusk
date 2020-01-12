
SET(_shader_input_location_index 0)
SET(_shader_input_locations "")

FUNCTION(ADD_SHADER_INPUT_LOCATION name)
    LIST(APPEND _shader_input_locations "${name}_INPUT_LOCATION ${_shader_input_location_index}")

    MATH(EXPR _shader_input_location_index "${_shader_input_location_index}+1")
    SET(_shader_input_location_index ${_shader_input_location_index} PARENT_SCOPE)
    SET(_shader_input_locations ${_shader_input_locations} PARENT_SCOPE)
ENDFUNCTION()

FUNCTION(GET_SHADER_INPUT_LOCATIONS out)
    SET(${out} "${_shader_input_locations}" PARENT_SCOPE)
ENDFUNCTION()

FUNCTION(WRITE_SHADER_INPUT_LOCATION_FILES cppout glslout)
    SET(SHADER_INPUT_LOCATIONS_DEFINES "")

    FOREACH(location ${_shader_input_locations})
        SET(SHADER_INPUT_LOCATIONS_DEFINES "${SHADER_INPUT_LOCATIONS_DEFINES}\n#define ${location}")
    ENDFOREACH()

    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/CMake/ShaderInputLocations.hpp.in
        ${cppout}
        @ONLY
    )

    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/CMake/ShaderInputLocations.inc.glsl.in
        ${glslout}
        @ONLY
    )
ENDFUNCTION()
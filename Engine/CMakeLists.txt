
INCLUDE(DuskModule)
INCLUDE(CompileShaders)
INCLUDE(SetSourceGroups)

SET(_TARGET DuskEngine)

DUSK_MODULE(${_TARGET} CORE)

LIST(APPEND MODULE_TARGETS ${_TARGET})
SET(MODULE_TARGETS ${MODULE_TARGETS} PARENT_SCOPE)

TARGET_LINK_LIBRARIES(
    ${_TARGET}
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
    global_shaders_in
    Assets/Shaders/*.glsl
)

LIST(APPEND SHADER_INCLUDE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets/Shaders/
    ${CMAKE_CURRENT_BINARY_DIR}/Assets/Shaders/
)

COMPILE_SHADERS("${global_shaders_in}" global_shaders_out)

TARGET_SOURCES(
    ${_TARGET}
    PRIVATE
    ${global_shaders_out}
)

SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${global_shaders_out}")

# Update the global RUNTIME_SEARCH_PATH to include paths to the
# module's .dll's
SET(RUNTIME_SEARCH_PATH ${RUNTIME_SEARCH_PATH} PARENT_SCOPE)

SET(SHADER_INCLUDE_PATH ${SHADER_INCLUDE_PATH} PARENT_SCOPE)
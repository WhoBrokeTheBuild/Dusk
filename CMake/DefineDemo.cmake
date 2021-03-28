
INCLUDE(CompileShaders)
INCLUDE(SetSourceGroups)

MACRO(DEFINE_DEMO _target)

    FILE(
        GLOB_RECURSE
        _sources
        "Source/*.h"
        "Source/*.hh"
        "Source/*.hpp"
        "Source/*.c"
        "Source/*.cc"
        "Source/*.cpp"
    )

    FILE(
        GLOB_RECURSE
        _sources_in
        "Source/*.in"
    )

    ###
    ### Template Files
    ###

    FOREACH(_file ${_sources_in})
        STRING(REPLACE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
            _file_out
            ${_file}
        )

        # Remove .in
        string(REGEX MATCH "^(.*)\\.[^.]*$" _file_out ${_file_out})
        set(_file_out ${CMAKE_MATCH_1})

        CONFIGURE_FILE(${_file} ${_file_out})
        LIST(APPEND _sources_out ${_file_out})
    ENDFOREACH()

    ###
    ### Shader Processing
    ###

    FILE(
        GLOB_RECURSE
        _shader_includes
        Assets/Shaders/*.inc.glsl
        Assets/Shaders/*.inc.hlsl
    )

    LIST(APPEND _shader_includes ${DUSK_ENGINE_SHADER_INCLUDES})

    FILE(
        GLOB_RECURSE
        _shaders_in
        Assets/Shaders/*.glsl
        Assets/Shaders/*.hlsl
    )

    LIST(INSERT DUSK_ASSET_PATH 0
        ${CMAKE_CURRENT_SOURCE_DIR}/Assets/
        ${CMAKE_CURRENT_BINARY_DIR}/Assets/
    )

    COMPILE_SHADERS("${_shader_includes}" "${_shaders_in}" _shaders_out)

    ###
    ### Asset Processing
    ###

    FILE(GLOB_RECURSE
        _assets
        Assets/*
    )

    # Stop VS from trying to "build" our .obj model files
    SET_SOURCE_FILES_PROPERTIES(
        ${_assets} 
        PROPERTIES
            HEADER_FILE_ONLY TRUE
    )

    ###
    ### Target Configuration
    ###

    ADD_EXECUTABLE(
        ${_target}
        ${_sources}
        ${_sources_out}
        ${_shaders_in}
        ${_shaders_out}
        ${_assets}
    )
    
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_sources}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${_sources_out}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_shaders_in}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${_shaders_out}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_assets}")

    TARGET_LINK_LIBRARIES(
        ${_target}
        PRIVATE
            DuskCore
    )

    TARGET_INCLUDE_DIRECTORIES(
        ${_target}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/Source
            ${CMAKE_CURRENT_BINARY_DIR}/Source
    )

    TARGET_COMPILE_DEFINITIONS(
        ${_target}
        PUBLIC
            # Disable VS "not secure" warnings
            $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
    )

    SET_TARGET_PROPERTIES(
        ${_target} 
        PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
    )

    INSTALL(FILES 
        $<TARGET_FILE:${_target}>
        DESTINATION bin
    )

    FILE(RELATIVE_PATH _folder ${CMAKE_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/..")
    SET_TARGET_PROPERTIES(
        ${_target}
        PROPERTIES
            FOLDER "${_folder}"
    )
    
    STRING(REPLACE " " "\\ " DUSK_MODULE_PATH "${DUSK_MODULE_PATH}")
    STRING(REPLACE " " "\\ " DUSK_ASSET_PATH "${DUSK_ASSET_PATH}")

    # TODO: Improve
    SET(_ext "")
    IF(WIN32)
        SET(_ext ".exe")
    ENDIF()

    FILE(RELATIVE_PATH _executable ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${_target}${_ext})
    FILE(TO_NATIVE_PATH ${_executable} _executable)

    EXECUTE_PROCESS(
        COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/Scripts/generate-launch-targets.py
            ${CMAKE_CURRENT_SOURCE_DIR}/${_target}.duskproj
            ${CMAKE_BINARY_DIR}
            ${_executable}
            "${DUSK_ASSET_PATH}"
            "${DUSK_MODULE_PATH}"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
ENDMACRO()

INCLUDE(CompileShaders)
INCLUDE(SetSourceGroups)

MACRO(DEFINE_DEMO _target)

    FILE(
        GLOB_RECURSE
        _sources
        Source/*.h
        Source/*.hh
        Source/*.hpp
        Source/*.c
        Source/*.cc
        Source/*.cpp
    )

    FILE(
        GLOB_RECURSE
        _sources_in
        Source/*.in
    )

    ###
    ### Template Files
    ###

    FOREACH(file ${_sources_in})
        STRING(REPLACE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
            file_out
            ${file}
        )

        string(REGEX MATCH "^(.*)\\.[^.]*$" file_out ${file_out})
        set(file_out ${CMAKE_MATCH_1})

        CONFIGURE_FILE(${file} ${file_out})
        LIST(APPEND _sources_out ${file_out})
    ENDFOREACH()

    ###
    ### Shader Processing
    ###

    FILE(
        GLOB_RECURSE
        _shaders_in
        Assets/Shaders/*.glsl
    )

    LIST(INSERT SHADER_INCLUDE_PATH 0
        ${CMAKE_CURRENT_SOURCE_DIR}/Assets/Shaders/
        ${CMAKE_CURRENT_BINARY_DIR}/Assets/Shaders/
    )

    COMPILE_SHADERS("${_shaders_in}" _shaders_out)

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
            DuskEngine
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

    TARGET_COMPILE_OPTIONS(
        ${_target}
        PUBLIC
            # Configure VS to use C++17, since it ignores CXX_STANDARD
            $<$<CXX_COMPILER_ID:MSVC>: /std:c++17>

            # Disable unknown pragmas warning, C++ exceptions
            $<$<CXX_COMPILER_ID:GNU>:   -Wall -Wno-unknown-pragmas -fno-exceptions>
            $<$<CXX_COMPILER_ID:Clang>: -Wall -Wno-unknown-pragmas -fno-exceptions>
            $<$<CXX_COMPILER_ID:MSVC>:  /MP /wd4068 /EHsc- /GR->
    )

    TARGET_LINK_OPTIONS(
        ${_target}
        PUBLIC
            # Fix windows bug in looking for python38.lib
            $<$<CXX_COMPILER_ID:MSVC>:/NODEFAULTLIB:python38.lib>
    )

    SET_TARGET_PROPERTIES(
        ${_target} 
        PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
    )

    INSTALL(FILES 
        $<TARGET_FILE:${_target}>
        DESTINATION bin
    )

    FILE(RELATIVE_PATH folder ${CMAKE_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/..")
    SET_TARGET_PROPERTIES(
        ${_target}
        PROPERTIES
            FOLDER "${folder}"
    )

    SET_TARGET_PROPERTIES(
        ${_target}
        PROPERTIES 
            VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${RUNTIME_SEARCH_PATH};$<$<CONFIG:Debug>:${RUNTIME_SEARCH_PATH_DEBUG}>;$<$<CONFIG:Release>:${RUNTIME_SEARCH_PATH_RELEASE}>\nDUSK_SHADER_INCLUDE_PATH=${SHADER_INCLUDE_PATH}"
    )

    STRING(JOIN ":" LD_LIBRARY_PATH ${RUNTIME_SEARCH_PATH})
    STRING(JOIN ":" SHADER_INCLUDE_PATH ${SHADER_INCLUDE_PATH})

    IF(NOT MSVC)
        ADD_CUSTOM_TARGET(
            run-${_target}
            COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" $<TARGET_FILE:${_target}>
            DEPENDS ${_target}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

        SET_TARGET_PROPERTIES(
            run-${_target}
            PROPERTIES 
                FOLDER "Automation"
        )

        IF(gdb_COMMAND)
            ADD_CUSTOM_TARGET(
                gdb-${_target}
                COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" "DUSK_SHADER_INCLUDE_PATH=${SHADER_INCLUDE_PATH}" gdb --args $<TARGET_FILE:${_target}>
                DEPENDS ${_target}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )

            SET_TARGET_PROPERTIES(
                gdb-${_target}
                PROPERTIES 
                    FOLDER "Automation"
            )
        ENDIF()

        IF(valgrind_COMMAND)
            ADD_CUSTOM_TARGET(
                valgrind-${_target}
                COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" "DUSK_SHADER_INCLUDE_PATH=${SHADER_INCLUDE_PATH}" valgrind $<TARGET_FILE:${_target}>
                DEPENDS ${_target}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )

            SET_TARGET_PROPERTIES(
                valgrind-${_target}
                PROPERTIES 
                    FOLDER "Automation"
            )
        ENDIF()
    ENDIF()
ENDMACRO()
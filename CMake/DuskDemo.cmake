
INCLUDE(CompileShaders)
INCLUDE(SetSourceGroups)

MACRO(DUSK_DEMO target)
    FILE(
        GLOB_RECURSE
        sources
        Source/*.h
        Source/*.hh
        Source/*.hpp
        Source/*.c
        Source/*.cc
        Source/*.cpp
    )

    FILE(
        GLOB_RECURSE
        sources_in
        Source/*.in
    )

    # Template Files

    FOREACH(file ${sources_in})
        STRING(REPLACE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
            file_out
            ${file}
        )

        string(REGEX MATCH "^(.*)\\.[^.]*$" file_out ${file_out})
        set(file_out ${CMAKE_MATCH_1})

        CONFIGURE_FILE(${file} ${file_out})
        LIST(APPEND sources_out ${file_out})
    ENDFOREACH()

    ###
    ### Shader Processing
    ###

    FILE(
        GLOB_RECURSE
        shaders_in
        Assets/Shaders/*.glsl
    )

    LIST(INSERT SHADER_INCLUDE_PATH 0
        ${CMAKE_CURRENT_SOURCE_DIR}/Assets/Shaders/
        ${CMAKE_CURRENT_BINARY_DIR}/Assets/Shaders/
    )

    COMPILE_SHADERS("${shaders_in}" shaders_out)

    # Asset Processing

    FILE(GLOB_RECURSE
        assets
        Assets/*
    )

    SET_SOURCE_FILES_PROPERTIES(
        ${assets} 
        PROPERTIES
            HEADER_FILE_ONLY TRUE
    )

    # Target Configuration

    ADD_EXECUTABLE(
        ${target}
        ${sources}
        ${sources_out}
        ${shaders_in}
        ${shaders_out}
        ${assets}
    )

    TARGET_LINK_LIBRARIES(
        ${target}
        DuskEngine
    )

    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${sources}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${sources_out}")
    
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${shaders_in}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${assets}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${shaders_out}")

    TARGET_INCLUDE_DIRECTORIES(
        ${target}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/Source
            ${CMAKE_CURRENT_BINARY_DIR}/Source
    )

    TARGET_COMPILE_DEFINITIONS(
        ${target}
        PUBLIC
            # Disable Visual Studio "not secure" warnings
            $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
    )

    TARGET_COMPILE_OPTIONS(
        ${target}
        PUBLIC
            # Disable unknown pragmas warning, C++ exceptions
            $<$<CXX_COMPILER_ID:GNU>:-Wall -Wno-unknown-pragmas -fno-exceptions>
            $<$<CXX_COMPILER_ID:Clang>:-Wall -Wno-unknown-pragmas -fno-exceptions>
            $<$<CXX_COMPILER_ID:MSVC>:/MP /wd4068 /EHsc- /GR->
    )

    TARGET_LINK_OPTIONS(
        ${target}
        PUBLIC
            # Fix windows bug in looking for python38.lib
            $<$<CXX_COMPILER_ID:MSVC>:/NODEFAULTLIB:python38.lib>
    )

    TARGET_COMPILE_FEATURES(
        ${target} 
        PRIVATE
            # Enable C++17 features
            cxx_std_17
    )

    SET_TARGET_PROPERTIES(
        ${target} 
        PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
    )

    INSTALL(FILES 
        $<TARGET_FILE:${target}>
        DESTINATION bin
    )

    FILE(RELATIVE_PATH folder ${CMAKE_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/..")
    SET_TARGET_PROPERTIES(
        ${target}
        PROPERTIES
            FOLDER "${folder}"
    )

    SET_TARGET_PROPERTIES(
        ${target}
        PROPERTIES 
            VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${RUNTIME_SEARCH_PATH};$<$<CONFIG:Debug>:${RUNTIME_SEARCH_PATH_DEBUG}>;$<$<CONFIG:Release>:${RUNTIME_SEARCH_PATH_RELEASE}>\nDUSK_SHADER_INCLUDE_PATH=${SHADER_INCLUDE_PATH}"
    )

    STRING(JOIN ":" LD_LIBRARY_PATH ${RUNTIME_SEARCH_PATH})
    STRING(JOIN ":" SHADER_INCLUDE_PATH ${SHADER_INCLUDE_PATH})

    ADD_CUSTOM_TARGET(
        run-${target}
        COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" "DUSK_SHADER_INCLUDE_PATH=${SHADER_INCLUDE_PATH}" $<TARGET_FILE:${target}>
        DEPENDS ${target}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    SET_TARGET_PROPERTIES(
        run-${target}
        PROPERTIES 
            FOLDER "Automation"
    )

    IF(GDB_COMMAND)
        ADD_CUSTOM_TARGET(
            gdb-${target}
            COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" "DUSK_SHADER_INCLUDE_PATH=${SHADER_INCLUDE_PATH}" gdb --args $<TARGET_FILE:${target}>
            DEPENDS ${target}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

        SET_TARGET_PROPERTIES(
            gdb-${target}
            PROPERTIES 
                FOLDER "Automation"
        )
    ENDIF()

    IF(LLDB_COMMAND)
        ADD_CUSTOM_TARGET(
            lldb-${target}
            COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" "DUSK_SHADER_INCLUDE_PATH=${SHADER_INCLUDE_PATH}" lldb --args $<TARGET_FILE:${target}>
            DEPENDS ${target}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

        SET_TARGET_PROPERTIES(
            lldb-${target}
            PROPERTIES 
                FOLDER "Automation"
        )
    ENDIF()

    IF(VALGRIND_COMMAND)
        ADD_CUSTOM_TARGET(
            valgrind-${target}
            COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" "DUSK_SHADER_INCLUDE_PATH=${SHADER_INCLUDE_PATH}" valgrind $<TARGET_FILE:${target}>
            DEPENDS ${target}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

        SET_TARGET_PROPERTIES(
            valgrind-${target}
            PROPERTIES 
                FOLDER "Automation"
        )
    ENDIF()
ENDMACRO()
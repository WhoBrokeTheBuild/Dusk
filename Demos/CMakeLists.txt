
# Demo Macro definition

MACRO(Demo target)
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

    FILE(
        GLOB_RECURSE
        shaders_in
        Shaders/*.glsl
    )

    IF(VulkanSDK_FOUND)
        FOREACH(file ${shaders_in})
            GET_FILENAME_COMPONENT(shader_name ${file} NAME_WLE)
            GET_FILENAME_COMPONENT(shader_path ${file} DIRECTORY)
            SET(shader_out "${shader_path}/${shader_name}.spv")

            GET_FILENAME_COMPONENT(shader_type ${shader_name} LAST_EXT)
            STRING(SUBSTRING ${shader_type} 1 -1 shader_type)

            ADD_CUSTOM_COMMAND(
                OUTPUT ${shader_out}
                COMMAND ${VulkanSDK_glslc_PROGRAM} -std=430core -fshader-stage=${shader_type} -o ${shader_out} ${file}
                DEPENDS ${file}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            )
            LIST(APPEND shaders_out ${shader_out})
        ENDFOREACH()
    ENDIF()

    ADD_EXECUTABLE(
        ${target}
        ${sources}
        ${sources_out}
        ${shaders_out}
    )

    TARGET_LINK_LIBRARIES(
        ${target}
        DuskCore
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
            $<$<CXX_COMPILER_ID:MSVC>:/MT>
            # Disable unknown pragmas warning, C++ exceptions
            $<$<CXX_COMPILER_ID:GNU>:-Wall -Wno-unknown-pragmas -fno-exceptions>
            $<$<CXX_COMPILER_ID:Clang>:-Wall -Wno-unknown-pragmas -fno-exceptions>
            $<$<CXX_COMPILER_ID:MSVC>:/MP /wd4068 /EHsc- /GR->
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

    IF(MSVC)
        SET_TARGET_PROPERTIES(
            ${target}
            PROPERTIES 
                VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${RUNTIME_SEARCH_PATH}"
        )
    ELSE()
        STRING(JOIN ":" LD_LIBRARY_PATH ${RUNTIME_SEARCH_PATH})

        ADD_CUSTOM_TARGET(
            run-${target}
            COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" $<TARGET_FILE:${target}>
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
                COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" gdb --args $<TARGET_FILE:${target}>
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
                COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" lldb --args $<TARGET_FILE:${target}>
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
                COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" valgrind $<TARGET_FILE:${target}>
                DEPENDS ${target}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )

            SET_TARGET_PROPERTIES(
                valgrind-${target}
                PROPERTIES 
                    FOLDER "Automation"
            )
        ENDIF()

    ENDIF()
ENDMACRO()

FIND_PROGRAM(GDB_COMMAND gdb)
FIND_PROGRAM(LLDB_COMMAND lldb)
FIND_PROGRAM(VALGRIND_COMMAND valgrind)

# Include Demos

ADD_SUBDIRECTORY(HelloWorld)

SET_PROPERTY(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "HelloWorld")

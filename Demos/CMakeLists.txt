
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
        Shaders/*.hlsl
        Shaders/*.vert
        #Shaders/*.tesc
        #Shaders/*.tese
        #Shaders/*.geom
        Shaders/*.frag
        #Shaders/*.comp
        #Shaders/*.mesh
        #Shaders/*.task
        #Shaders/*.rgen
        #Shaders/*.rint
        #Shaders/*.rahit
        #Shaders/*.rchit
        #Shaders/*.rmiss
        #Shaders/*.rcall
    )

    SET(shader_ids "")
    FOREACH(file ${shaders_in})
        GET_FILENAME_COMPONENT(shader_name ${file} NAME_WE)
        GET_FILENAME_COMPONENT(shader_path ${file} DIRECTORY)
        FILE(RELATIVE_PATH shader_path ${CMAKE_SOURCE_DIR} ${shader_path})
        
        STRING(MAKE_C_IDENTIFIER ${shader_name} shader_id)

        LIST(APPEND shader_ids ${shader_id})
        LIST(APPEND shader_files_${shader_id} ${file})
        SET(shader_out_${shader_id} "${shader_path}/${shader_name}.spv")
    ENDFOREACH()

    LIST(REMOVE_DUPLICATES shader_ids)

    SET(shaders_out "")
    FOREACH(id ${shader_ids})
        ADD_CUSTOM_COMMAND(
            OUTPUT ${shader_out_${id}}
            COMMAND ${VulkanSDK_glslangValidator_PROGRAM} -o ${shader_out_${id}} -V ${shader_files_${id}}
            DEPENDS ${shader_files_${id}}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
        LIST(APPEND shaders_out ${shader_out_${id}})
    ENDFOREACH()

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

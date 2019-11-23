
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

    ADD_EXECUTABLE(
        ${target}
        ${sources}
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
            $<$<CXX_COMPILER_ID:MSVC>:/MP /wd4068 /EHsc->
    )

    TARGET_LINK_OPTIONS(
        ${target}
        PUBLIC
            # Disable Visual Studio default libraries
            $<$<CXX_COMPILER_ID:MSVC>:/NODEFAULTLIB:MSVCRT /NODEFAULTLIB:LIBCMT>
    )

    TARGET_COMPILE_FEATURES(
        ${target} 
        PRIVATE
            # Enable C++17 features
            cxx_std_17
    )

    SET_TARGET_PROPERTIES(
        ${target} PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
    )

    FILE(RELATIVE_PATH folder ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    SET_TARGET_PROPERTIES(
        ${target}
        PROPERTIES FOLDER "${folder}"
    )

    IF(MSVC)
        SET_TARGET_PROPERTIES(
            ${target} PROPERTIES 
            VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${CMAKE_MSVCIDE_RUN_PATH}"
        )
    ENDIF()

    ADD_CUSTOM_TARGET(
        run-${target}
        COMMAND $<TARGET_FILE:${target}>
        DEPENDS ${target}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

ENDMACRO()

# Include Demos

ADD_SUBDIRECTORY(HelloWorld)

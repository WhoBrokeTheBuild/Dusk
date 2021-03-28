
INCLUDE(SetSourceGroups)
INCLUDE(GoogleTest)

MACRO(DEFINE_MODULE _target _prefix)

    FILE(
        GLOB_RECURSE
        _sources
        "Public/*.h"
        "Public/*.hh"
        "Public/*.hpp"
        "Private/*.h"
        "Private/*.hh"
        "Private/*.hpp"
        "Private/*.c"
        "Private/*.cc"
        "Private/*.cpp"
    )

    FILE(GLOB_RECURSE
        _sources_in
        "Public/*.in"
        "Private/*.in"
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

    LIST(APPEND DUSK_ASSET_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/Assets/
        ${CMAKE_CURRENT_BINARY_DIR}/Assets/
    )
    
    SET(DUSK_ASSET_PATH ${DUSK_ASSET_PATH} PARENT_SCOPE)

    ###
    ### Target Configuration
    ###
    
    ADD_LIBRARY(
        ${_target} SHARED
        ${_sources}
        ${_sources_in}
        ${_sources_out}
    )
    
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_sources}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_sources_in}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${_sources_out}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_assets}")
    
    IF(NOT _target STREQUAL "DuskCore")
        TARGET_LINK_LIBRARIES(
            ${_target}
            PUBLIC
                DuskCore
        )
    ENDIF()

    TARGET_INCLUDE_DIRECTORIES(
        ${_target}
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/Public>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/Private
            ${CMAKE_CURRENT_BINARY_DIR}/Private
    )

    TARGET_COMPILE_DEFINITIONS(
        ${_target}
        PUBLIC
            # Disable Visual Studio "not secure" warnings
            $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
    )

    TARGET_COMPILE_OPTIONS(
        ${_target}
        PUBLIC
            # Configure exception handling model
            $<$<CXX_COMPILER_ID:MSVC>: /EHs>

            # Build with multiple processors
            $<$<CXX_COMPILER_ID:MSVC>: /MP>

            # Force windows to use UTF-8
            $<$<CXX_COMPILER_ID:MSVC>: /utf-8>

            # Enable most warnings, disable unknown pragmas warning
            $<$<CXX_COMPILER_ID:GNU>:   -Wall -Wno-unknown-pragmas>
            $<$<CXX_COMPILER_ID:Clang>: -Wall -Wno-unknown-pragmas>
            $<$<CXX_COMPILER_ID:MSVC>:  /wd4068>
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
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            DEFINE_SYMBOL "DUSK_${_prefix}_EXPORT"
    )
    
    INSTALL(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Public/
        DESTINATION include
        PATTERN "*.in" EXCLUDE
    )

    FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Public)

    INSTALL(
        DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Public/
        DESTINATION include
        PATTERN "*.in" EXCLUDE
    )

    FILE(RELATIVE_PATH folder ${CMAKE_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/..")
    SET_TARGET_PROPERTIES(
        ${_target}
        PROPERTIES 
            FOLDER "${folder}"
    )

    LIST(APPEND DUSK_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})
    SET(DUSK_MODULE_PATH ${DUSK_MODULE_PATH} PARENT_SCOPE)
    
    LIST(APPEND DUSK_MODULE_TARGETS ${_target})
    SET(DUSK_MODULE_TARGETS ${DUSK_MODULE_TARGETS} PARENT_SCOPE)
    
    ###
    ### Testing
    ###

    IF(BUILD_TESTS)
        FILE(GLOB_RECURSE
            _tests
            "Tests/*Test.cpp"
        )
        
        FOREACH(_test_source ${_tests})
            GET_FILENAME_COMPONENT(_test_target ${_test_source} NAME_WE)
            
            ADD_EXECUTABLE(
                ${_test_target}
                ${_test_source}
            )

            TARGET_LINK_LIBRARIES(
                ${_test_target}
                PRIVATE
                    ${_target}
                    GTest::GTest
                    GTest::Main
            )

            SET_TARGET_PROPERTIES(
                ${_test_target}
                PROPERTIES
                    CXX_STANDARD 20
                    CXX_STANDARD_REQUIRED ON
                    CXX_EXTENSIONS OFF
            )

            SET_TARGET_PROPERTIES(
                ${_test_target}
                PROPERTIES FOLDER "${folder}/Tests"
            )

            SET_TARGET_PROPERTIES(
                ${_test_target}
                PROPERTIES 
                    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${DUSK_MODULE_PATH};$<$<CONFIG:Debug>:${DUSK_MODULE_PATH_DEBUG}>;$<$<CONFIG:Release>:${DUSK_MODULE_PATH_RELEASE}>"
            )

            GTEST_ADD_TESTS(
                TARGET ${_test_target}
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Tests"
            )
        ENDFOREACH()
    ENDIF()
ENDMACRO()
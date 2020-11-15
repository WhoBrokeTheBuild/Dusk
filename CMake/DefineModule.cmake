
INCLUDE(SetSourceGroups)

MACRO(DEFINE_MODULE _target _prefix)

    FILE(
        GLOB_RECURSE
        _public
        Public/*.h
        Public/*.hh
        Public/*.hpp
    )

    FILE(GLOB_RECURSE
        _public_in
        Public/*.in
    )

    FILE(
        GLOB_RECURSE
        _private
        Private/*.h
        Private/*.hh
        Private/*.hpp
        Private/*.c
        Private/*.cc
        Private/*.cpp
    )

    FILE(GLOB_RECURSE
        _private_in
        Private/*.in
    )

    ###
    ### Template Files
    ###

    FOREACH(file ${_public_in})
        STRING(REPLACE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
            file_out
            ${file}
        )

        string(REGEX MATCH "^(.*)\\.[^.]*$" file_out ${file_out})
        set(file_out ${CMAKE_MATCH_1})

        CONFIGURE_FILE(${file} ${file_out})
        LIST(APPEND _public_out ${file_out})
    ENDFOREACH()
        
    FOREACH(file ${_private_in})
        STRING(REPLACE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
            file_out
            ${file}
        )

        string(REGEX MATCH "^(.*)\\.[^.]*$" file_out ${file_out})
        set(file_out ${CMAKE_MATCH_1})

        CONFIGURE_FILE(${file} ${file_out})
        LIST(APPEND _private_out ${file_out})
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

    ###
    ### Target Configuration
    ###
    
    ADD_LIBRARY(
        ${_target} SHARED
        ${_public}
        ${_private}
        ${_assets}
    )

    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_public}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${_public_out}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_private}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${_private_out}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_assets}")
    
    IF(NOT _target STREQUAL "DuskEngine")
        TARGET_LINK_LIBRARIES(
            ${_target}
            PUBLIC
                DuskEngine
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
        PRIVATE
            DUSK_${_prefix}_EXPORT
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

    IF(MSVC)
        LIST(APPEND RUNTIME_SEARCH_PATH "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)")
    ELSE()
        LIST(APPEND RUNTIME_SEARCH_PATH "${CMAKE_CURRENT_BINARY_DIR}")
    ENDIF()

    SET(RUNTIME_SEARCH_PATH ${RUNTIME_SEARCH_PATH} PARENT_SCOPE)
    
    LIST(APPEND MODULE_TARGETS ${_target})
    SET(MODULE_TARGETS ${MODULE_TARGETS} PARENT_SCOPE)    

    # Tests

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

            SET_TARGET_PROPERTIES(
                ${_test_target} PROPERTIES
                    CXX_STANDARD 17
                    CXX_STANDARD_REQUIRED ON
                    CXX_EXTENSIONS OFF
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
                PROPERTIES FOLDER "${folder}/Tests"
            )

            SET_TARGET_PROPERTIES(
                ${_test_target}
                PROPERTIES 
                    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${RUNTIME_SEARCH_PATH};$<$<CONFIG:Debug>:${RUNTIME_SEARCH_PATH_DEBUG}>;$<$<CONFIG:Release>:${RUNTIME_SEARCH_PATH_RELEASE}>"
            )

            ADD_TEST(
                NAME ${_test_target}
                COMMAND $<TARGET_FILE:${_test_target}>
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Tests"
            )
        
        ENDFOREACH()
    ENDIF()
ENDMACRO()
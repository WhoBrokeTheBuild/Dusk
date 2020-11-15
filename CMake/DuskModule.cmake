
INCLUDE(SetSourceGroups)

MACRO(DUSK_MODULE target prefix)
    ADD_LIBRARY(${target} SHARED "")

    FILE(
        GLOB_RECURSE
        public
        Public/*.h
        Public/*.hh
        Public/*.hpp
    )

    FILE(GLOB_RECURSE
        public_in
        Public/*.in
    )

    FILE(
        GLOB_RECURSE
        private
        Private/*.h
        Private/*.hh
        Private/*.hpp
        Private/*.c
        Private/*.cc
        Private/*.cpp
    )

    FILE(GLOB_RECURSE
        private_in
        Private/*.in
    )

    # Template Files

    FOREACH(file ${public_in})
        STRING(REPLACE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
            file_out
            ${file}
        )

        string(REGEX MATCH "^(.*)\\.[^.]*$" file_out ${file_out})
        set(file_out ${CMAKE_MATCH_1})

        CONFIGURE_FILE(${file} ${file_out})
        LIST(APPEND public_out ${file_out})
    ENDFOREACH()
        
    FOREACH(file ${private_in})
        STRING(REPLACE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
            file_out
            ${file}
        )

        string(REGEX MATCH "^(.*)\\.[^.]*$" file_out ${file_out})
        set(file_out ${CMAKE_MATCH_1})

        CONFIGURE_FILE(${file} ${file_out})
        LIST(APPEND private_out ${file_out})
    ENDFOREACH()

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
    
    TARGET_INCLUDE_DIRECTORIES(
        ${target}
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/Public>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/Private
            ${CMAKE_CURRENT_BINARY_DIR}/Private
    )

    TARGET_SOURCES(
        ${target}
        PRIVATE
            ${public}
            ${private}
            ${assets}
    )

    TARGET_COMPILE_DEFINITIONS(
        ${target}
        PUBLIC
            # Disable Visual Studio "not secure" warnings
            $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
        PRIVATE
            DUSK_${prefix}_EXPORT
    )

    TARGET_COMPILE_OPTIONS(
        ${target}
        PUBLIC
            # Disable unknown pragmas warning, C++ exceptions
            $<$<CXX_COMPILER_ID:GNU>:-Wall -Wno-unknown-pragmas -fno-exceptions>
            $<$<CXX_COMPILER_ID:Clang>:-Wall -Wno-unknown-pragmas -fno-exceptions>
            $<$<CXX_COMPILER_ID:MSVC>:/MP /wd4068 /EHsc- /GR- /std:c++17>
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
        ${target} PROPERTIES
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
        ${target}
        PROPERTIES 
            FOLDER "${folder}"
    )

    IF(MSVC)
        LIST(APPEND RUNTIME_SEARCH_PATH "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)")
    ELSE()
        LIST(APPEND RUNTIME_SEARCH_PATH "${CMAKE_CURRENT_BINARY_DIR}")
    ENDIF()

    SET(RUNTIME_SEARCH_PATH ${RUNTIME_SEARCH_PATH} PARENT_SCOPE)

    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${assets}")
    
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${public}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${public_out}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${private}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${private_out}")

    # Tests

    IF(BUILD_TESTS)
        FILE(GLOB_RECURSE
            tests
            "Tests/*Test.cpp"
        )
        
        FOREACH(test_source ${tests})
            GET_FILENAME_COMPONENT(test_target ${test_source} NAME_WE)
            
            ADD_EXECUTABLE(
                ${test_target}
                ${test_source}
            )

            TARGET_COMPILE_FEATURES(
                ${test_target} 
                PRIVATE
                    # Enable C++17 features
                    cxx_std_17
            )
        
            SET_TARGET_PROPERTIES(
                ${test_target} PROPERTIES
                    CXX_STANDARD 17
                    CXX_STANDARD_REQUIRED ON
                    CXX_EXTENSIONS OFF
            )

            TARGET_LINK_LIBRARIES(
                ${test_target}
                PRIVATE
                    ${target}
                    GTest::GTest
                    GTest::Main
            )

            SET_TARGET_PROPERTIES(
                ${test_target}
                PROPERTIES FOLDER "${folder}/Tests"
            )

            ADD_TEST(
                NAME ${test_target}
                COMMAND $<TARGET_FILE:${test_target}>
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Tests"
            )
        ENDFOREACH()
    ENDIF()
ENDMACRO()
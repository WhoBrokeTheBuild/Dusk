
###
### Module Macro definition
###

MACRO(Module target prefix)
    ADD_LIBRARY(${target} SHARED "")

    FILE(
        GLOB_RECURSE
        public
        Public/*.h
        Public/*.hh
        Public/*.hpp
        Public/*.c
        Public/*.cc
        Public/*.cpp
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

    TARGET_INCLUDE_DIRECTORIES(
        ${target}
        PRIVATE "Private/"
        PUBLIC "Public/"
    )

    TARGET_SOURCES(
        ${target}
        PRIVATE ${private} ${public}
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
            $<$<CXX_COMPILER_ID:MSVC>:/MP /wd4068 /EHsc- /GR->
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

    FILE(RELATIVE_PATH folder ${CMAKE_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/..")
    SET_TARGET_PROPERTIES(
        ${target}
        PROPERTIES 
            FOLDER "${folder}"
    )

    IF(MSVC)
        LIST(APPEND CMAKE_MSVCIDE_RUN_PATH
            "${CMAKE_CURRENT_BINARY_DIR}/Debug"
            "${CMAKE_CURRENT_BINARY_DIR}/Release"
        )
        SET(CMAKE_MSVCIDE_RUN_PATH ${CMAKE_MSVCIDE_RUN_PATH} PARENT_SCOPE)
        SET_TARGET_PROPERTIES(
            ${target} PROPERTIES 
            VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${CMAKE_MSVCIDE_RUN_PATH}"
        )
    ENDIF()

    FOREACH(file ${public} ${private})
        FILE(RELATIVE_PATH file_path ${CMAKE_CURRENT_SOURCE_DIR} ${file})
        GET_FILENAME_COMPONENT(folder ${file_path} DIRECTORY)
        FILE(TO_NATIVE_PATH ${folder} folder)
        SOURCE_GROUP(${folder} FILES ${file})
    ENDFOREACH()

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

            TARGET_LINK_LIBRARIES(
                ${test_target}
                PRIVATE
                    ${target}
                    GTest::GTest
                    GTest::Main
                    $<$<CXX_COMPILER_ID:MSVC>:MSVCRT>
            )

            SET_TARGET_PROPERTIES(
                ${test_target}
                PROPERTIES FOLDER "${folder}/Tests"
            )

            ADD_TEST(
                NAME ${test_target}
                COMMAND ${test_target}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
        ENDFOREACH()
    ENDIF()
ENDMACRO()

###
### Include Modules
###

ADD_SUBDIRECTORY(Core)

# Graphics Drivers

ADD_SUBDIRECTORY(OpenGL)

# Loaders

ADD_SUBDIRECTORY(STBI)

# Update the global CMAKE_MSVCIDE_RUN_PATH to include paths to the
# module's .dll's
SET(CMAKE_MSVCIDE_RUN_PATH ${CMAKE_MSVCIDE_RUN_PATH} PARENT_SCOPE)

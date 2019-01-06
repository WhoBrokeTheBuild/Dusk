SET(_ENGINE "Dusk")

SET(_ENGINE_CONFIG_IN  "${CMAKE_CURRENT_SOURCE_DIR}/include/dusk/Config.hpp.in")
SET(_ENGINE_CONFIG_OUT "${CMAKE_CURRENT_BINARY_DIR}/include/dusk/Config.hpp")

CONFIGURE_FILE(${_ENGINE_CONFIG_IN} ${_ENGINE_CONFIG_OUT})

FILE(GLOB_RECURSE 
    _ENGINE_ASSETS
    "${CMAKE_SOURCE_DIR}/Assets/*"
)

FILE(GLOB_RECURSE 
    _ENGINE_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

FILE(GLOB_RECURSE 
    _ENGINE_SOURCES  
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

IF(WIN32)
    ADD_LIBRARY(
        ${_ENGINE}
        ${_ENGINE_INCLUDES}
        ${_ENGINE_SOURCES}
        ${_ENGINE_CONFIG_OUT}
        ${_ENGINE_ASSETS}
    )
ELSE()
    ADD_LIBRARY(
        ${_ENGINE} SHARED 
        ${_ENGINE_INCLUDES}
        ${_ENGINE_SOURCES}
        ${_ENGINE_CONFIG_OUT}
        ${_ENGINE_ASSETS}
    )
ENDIF()

TARGET_INCLUDE_DIRECTORIES(
    ${_ENGINE}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_BINARY_DIR}/include"
)

TARGET_COMPILE_FEATURES(
    ${_ENGINE} 
    PRIVATE
        cxx_std_17
        cxx_auto_type
)

TARGET_COMPILE_OPTIONS(
    ${_ENGINE} 
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wno-unknown-pragmas>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wno-unknown-pragmas>
        $<$<CXX_COMPILER_ID:MSVC>:-wd4068>
)

TARGET_LINK_LIBRARIES(
    ${_ENGINE}
    GLM::GLM
    JSON::JSON
    SDL2::SDL2
    OpenAL::OpenAL
)

IF(MSVC)
    SET_TARGET_PROPERTIES(
        ${_ENGINE} PROPERTIES
        LINK_FLAGS "/NODEFAULTLIB:LIBCMT /NODEFAULTLIB:MSVCRT" 
    )
ENDIF()

FOREACH(file IN ITEMS ${_ENGINE_ASSETS})
    FILE(RELATIVE_PATH file_path ${CMAKE_SOURCE_DIR} ${file})
    GET_FILENAME_COMPONENT(file_path ${file_path} DIRECTORY)
    FILE(TO_NATIVE_PATH ${file_path} file_path)
    SOURCE_GROUP(${file_path} FILES ${file})
ENDFOREACH()

FOREACH(file IN ITEMS ${_ENGINE_INCLUDES} ${_ENGINE_SOURCES})
    FILE(RELATIVE_PATH file_path ${CMAKE_CURRENT_SOURCE_DIR} ${file})
    GET_FILENAME_COMPONENT(file_path ${file_path} DIRECTORY)
    FILE(TO_NATIVE_PATH ${file_path} file_path)
    SOURCE_GROUP(${file_path} FILES ${file})
ENDFOREACH()

FOREACH(file IN ITEMS ${_ENGINE_CONFIG_OUT})
    FILE(RELATIVE_PATH file_path ${CMAKE_CURRENT_BINARY_DIR} ${file})
    GET_FILENAME_COMPONENT(file_path ${file_path} DIRECTORY)
    FILE(TO_NATIVE_PATH ${file_path} file_path)
    SOURCE_GROUP(${file_path} FILES ${file})
ENDFOREACH()


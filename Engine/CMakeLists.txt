
SET(_target DuskEngine)

FILE(
    GLOB_RECURSE
    _sources
    "Source/*.h"
    "Source/*.hh"
    "Source/*.hpp"
    "Source/*.c"
    "Source/*.cc"
    "Source/*.cpp"
)

ADD_EXECUTABLE(
    ${_target}
    ${_sources}
)
    
SET_TARGET_PROPERTIES(
    ${_target}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        DuskCore
)


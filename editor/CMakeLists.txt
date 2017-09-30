
###
### Project Configuration
###

SET(_EDITOR_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui
)
SET(_EDITOR_INCLUDE_DIRS "${_EDITOR_INCLUDE_DIRS}" PARENT_SCOPE)

SET(_DUSK_EDITOR "Dusk-Editor")

SET(_EDITOR_INCLUDES
    include/Main.hpp
    include/Editor.hpp

    # imgui
    include/imgui/imgui.h
    include/imgui/imgui_internal.h
    include/imgui/imgui_impl_sdl_gl3.h
)

SET(_EDITOR_SOURCES
    src/Editor.cpp

    # imgui
    src/imgui/imgui.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_demo.cpp
    src/imgui/imgui_impl_sdl_gl3.cpp
)

INCLUDE_DIRECTORIES(
    ${_ENGINE_INCLUDE_DIRS}
    ${_EDITOR_INCLUDE_DIRS}
    ${_DEPEND_INCLUDE_DIRS}
)

ADD_LIBRARY(${_DUSK_EDITOR} SHARED
    ${_EDITOR_INCLUDES}
    ${_EDITOR_SOURCES}
)

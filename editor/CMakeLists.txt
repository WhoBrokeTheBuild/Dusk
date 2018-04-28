
###
### Project Configuration
###

ADD_DEFINITIONS(
    -DIMGUI_DISABLE_INCLUDE_IMCONFIG_H
    -DIMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
    -DIMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
)

SET(_EDITOR_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${_EDITOR_DEPEND_INCLUDE_DIRS}
)
SET(_EDITOR_INCLUDE_DIRS "${_EDITOR_INCLUDE_DIRS}" PARENT_SCOPE)

SET(_DUSK_EDITOR "Dusk-Editor")

SET(_EDITOR_INCLUDES
    include/ActorWindow.hpp
    include/Config.hpp
    include/DemoWindow.hpp
    include/Editor.hpp
    include/EditorWindow.hpp
    include/ImBind.hpp
    include/PopupWindow.hpp
    include/SceneWindow.hpp
    include/SettingsWindow.hpp
    include/ShadersWindow.hpp
)

SET(_EDITOR_SOURCES
    src/ActorWindow.cpp
    src/Editor.cpp
    src/EditorWindow.cpp
    src/ImBind.cpp
    src/PopupWindow.cpp
    src/SceneWindow.cpp
    src/SettingsWindow.cpp
    src/ShadersWindow.cpp
)

INCLUDE_DIRECTORIES(
    ${_ENGINE_INCLUDE_DIRS}
    ${_EDITOR_INCLUDE_DIRS}
    ${_DEPEND_INCLUDE_DIRS}
)

IF(WIN32)
    ADD_LIBRARY(${_DUSK_EDITOR}
        ${_EDITOR_INCLUDES}
        ${_EDITOR_SOURCES}
        ${_EDITOR_DEPEND_SOURCES}
    )
ELSE()
    ADD_LIBRARY(${_DUSK_EDITOR} SHARED
        ${_EDITOR_INCLUDES}
        ${_EDITOR_SOURCES}
        ${_EDITOR_DEPEND_SOURCES}
    )
ENDIF()

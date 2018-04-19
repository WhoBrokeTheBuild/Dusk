
###
### Options
###

OPTION(VERBOSE_LOGGING "Build Dusk with Verbose Logging enabled" OFF)
IF(VERBOSE_LOGGING)
    ADD_DEFINITIONS(-DDUSK_ENABLE_VERBOSE_LOGGING)
ENDIF()

OPTION(BINARY_SHADERS "Build Dusk with Binary Shader support" OFF)
IF(BINARY_SHADERS)
    ADD_DEFINITIONS(-DDUSK_ENABLE_BINARY_SHADERS)
ENDIF()

SET(MIN_OPENGL_VERSION "3.3" CACHE STRING "Minimum supported OpenGL Version")

STRING(REPLACE "." ";" VERSION_LIST ${MIN_OPENGL_VERSION})
LIST(GET VERSION_LIST 0 MIN_OPENGL_VERSION_MAJOR)
LIST(GET VERSION_LIST 1 MIN_OPENGL_VERSION_MINOR)

ADD_DEFINITIONS(-DDUSK_MIN_OPENGL_VERSION_MAJOR=${MIN_OPENGL_VERSION_MAJOR}
                -DDUSK_MIN_OPENGL_VERSION_MINOR=${MIN_OPENGL_VERSION_MINOR})

###
### Project Configuration
###

SET(_ENGINE_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/stb
)
SET(_ENGINE_INCLUDE_DIRS "${_ENGINE_INCLUDE_DIRS}" PARENT_SCOPE)

SET(_DUSK_ENGINE "Dusk")

SET(_DUSK_CONFIG_IN ${CMAKE_CURRENT_SOURCE_DIR}/include/dusk/Config.hpp.in)
SET(_DUSK_CONFIG    ${CMAKE_CURRENT_BINARY_DIR}/include/dusk/Config.hpp)

SET(_DUSK_INCLUDES
    include/dusk/Main.hpp
    include/dusk/Dusk.hpp
    include/dusk/core/App.hpp
    include/dusk/core/Benchmark.hpp
    include/dusk/core/Event.hpp
    include/dusk/core/Log.hpp
    include/dusk/core/Platform.hpp
    include/dusk/core/ScriptHost.hpp
    #include/dusk/core/Timer.hpp
    include/dusk/core/Util.hpp
    #include/dusk/asset/Font.hpp
    include/dusk/asset/Material.hpp
    include/dusk/asset/Mesh.hpp
    include/dusk/asset/Shader.hpp
    include/dusk/asset/Shader.inc.hpp
    #include/dusk/asset/Sound.hpp
    include/dusk/asset/Texture.hpp
    #include/dusk/asset/Video.hpp
    include/dusk/scene/Actor.hpp
    include/dusk/scene/Camera.hpp
    include/dusk/scene/IComponent.hpp
    include/dusk/scene/Scene.hpp
    #include/dusk/scene/UI.hpp
)

SET(_DUSK_SOURCES
    src/dusk/Dusk.cpp
    src/dusk/core/App.cpp
    src/dusk/core/ScriptHost.cpp
    src/dusk/core/Util.cpp
    #src/dusk/asset/Font.cpp
    src/dusk/asset/Material.cpp
    src/dusk/asset/Mesh.cpp
    src/dusk/asset/Shader.cpp
    #src/dusk/asset/Sound.cpp
    src/dusk/asset/Texture.cpp
    #src/dusk/asset/Video.cpp
    src/dusk/scene/Actor.cpp
    src/dusk/scene/Camera.cpp
    src/dusk/scene/IComponent.cpp
    src/dusk/scene/Scene.cpp
    #src/dusk/scene/UI.cpp
)

###
### Build Directives
###

CONFIGURE_FILE(${_DUSK_CONFIG_IN} ${_DUSK_CONFIG})

INCLUDE_DIRECTORIES(
    ${_ENGINE_INCLUDE_DIRS}
    ${_DEPEND_INCLUDE_DIRS}
)

IF(WIN32)
    ADD_LIBRARY(${_DUSK_ENGINE}
        ${_DUSK_CONFIG}
        ${_DUSK_INCLUDES}
        ${_DUSK_SOURCES}
        ${_DEPEND_SOURCES}
    )
ELSE()
    ADD_LIBRARY(${_DUSK_ENGINE} SHARED
        ${_DUSK_CONFIG}
        ${_DUSK_INCLUDES}
        ${_DUSK_SOURCES}
        ${_DEPEND_SOURCES}
    )
ENDIF()

ADD_DEPENDENCIES(${_DUSK_ENGINE} depend)

###
### Link Libraries
###

# Configuration-independant libraries
TARGET_LINK_LIBRARIES(
    ${_DUSK_ENGINE}
    ${_DEPEND_LIBRARIES}
)

# Debug-only libraries
FOREACH(debug_lib IN ITEMS ${_DEPEND_LIBRARIES_DEBUG})
    TARGET_LINK_LIBRARIES(
        ${_DUSK_ENGINE}
        debug ${debug_lib}
    )
ENDFOREACH()

# Release-only libraries
FOREACH(release_lib IN ITEMS ${_DEPEND_LIBRARIES_RELEASE})
    TARGET_LINK_LIBRARIES(
        ${_DUSK_ENGINE}
        optimized ${release_lib}
    )
ENDFOREACH()

###
### Set IDE folders
###

FOREACH(file IN ITEMS ${Dusk_ASSETS} ${Dusk_INCLUDES} ${Dusk_SOURCES})
    GET_FILENAME_COMPONENT(file_path ${file} DIRECTORY)
    FILE(TO_NATIVE_PATH ${file_path} file_path)
    SOURCE_GROUP(${file_path} FILES ${file})
ENDFOREACH()

FILE(TO_NATIVE_PATH "include/dusk" config_path)
SOURCE_GROUP(${config_path} FILES ${Dusk_CONFIG})

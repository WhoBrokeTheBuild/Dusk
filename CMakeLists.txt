CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)
CMAKE_POLICY(SET CMP0048 NEW)

PROJECT(Dusk VERSION 0.0.2)

# Add dependencies to be built with the project
ADD_SUBDIRECTORY(depend)

### Global configuration

# Allow for custom organization of files in VisualStudio
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Allow for custom FindXXX.cmake files
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Get Git revision
INCLUDE(GetGitRevisionDescription)
GET_GIT_HEAD_REVISION(GIT_REFSPEC DUSK_REVISION)
STRING(SUBSTRING "${DUSK_REVISION}" 0 12 DUSK_REVISION)

# Options
OPTION(VERBOSE_LOGGING "Build Dusk with Verbose Logging enabled" OFF)
IF(VERBOSE_LOGGING)
    ADD_DEFINITIONS(-DDUSK_ENABLE_VERBOSE_LOGGING)
ENDIF()

OPTION(BINARY_SHADERS "Build Dusk with Binary Shader support" ON)
IF(BINARY_SHADERS)
    ADD_DEFINITIONS(-DDUSK_ENABLE_BINARY_SHADERS)
ENDIF()

### Compiler-specific flags

# GCC or Clang
IF(CMAKE_COMPILER_IS_GNUCC OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")

    INCLUDE(CheckCXXCompilerFlag)

    CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
    CHECK_CXX_COMPILER_FLAG("-std=c++1y" COMPILER_SUPPORTS_CXX1Y)

    IF(COMPILER_SUPPORTS_CXX14)
        SET(CMAKE_CXX_FLAGS "-std=c++14")
    ELSEIF(COMPILER_SUPPORTS_CXX1Y)
        SET(CMAKE_CXX_FLAGS "-std=c++1y")
    ELSE()
        MESSAGE(FATAL_ERROR "Compiler '${CMAKE_CXX_COMPILER}' has no C++14 support")
    ENDIF()

    ADD_DEFINITIONS(-D_XOPEN_SOURCE=700)
    ADD_COMPILE_OPTIONS(-Wall -Wextra -pedantic -Wno-unused-parameter
        -Wno-missing-braces -Wno-unused-function -Wno-variadic-macros)

# VisualStudio
ELSEIF(MSVC)

    ADD_DEFINITIONS(/D_CRT_SECURE_NO_WARNINGS)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
    SET(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")

ENDIF()

# Clang
IF(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    ADD_COMPILE_OPTIONS(-Wno-gnu-zero-variadic-macro-arguments
        -Wno-gnu-statement-expression -Wno-keyword-macro)
ENDIF()

### Required packages

FIND_PACKAGE(OpenGL      REQUIRED)

INCLUDE_DIRECTORIES(BEFORE SYSTEM
    ${OPENGL_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${FLATBUFFERS_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${GSL_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIRS}
    ${OPENAL_INCLUDE_DIRS}
    ${OGG_INCLUDE_DIRS}
    ${VORBIS_INCLUDE_DIRS}
    ${THEORA_INCLUDE_DIRS}
)

SET(Dusk_LINK_LIBRARIES
    ${OPENGL_LIBRARIES}
    ${LUA_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${SDL2_LIBRARIES}
    ${FLATBUFFERS_LIBRARIES}
    ${OPENAL_LIBRARIES}
    ${OGG_LIBRARIES}
    ${VORBIS_LIBRARIES}
    ${THEORA_LIBRARIES}
)
SET(Dusk_LINK_LIBRARIES_DEBUG
    ${LUA_LIBRARIES_DEBUG}
    ${ZLIB_LIBRARIES_DEBUG}
    ${SDL2_LIBRARIES_DEBUG}
    ${FLATBUFFERS_LIBRARIES_DEBUG}
    ${OPENAL_LIBRARIES_DEBUG}
    ${OGG_LIBRARIES_DEBUG}
    ${VORBIS_LIBRARIES_DEBUG}
    ${THEORA_LIBRARIES_DEBUG}
)
SET(Dusk_LINK_LIBRARIES_RELEASE
    ${LUA_LIBRARIES_RELEASE}
    ${ZLIB_LIBRARIES_RELEASE}
    ${SDL2_LIBRARIES_RELEASE}
    ${FLATBUFFERS_LIBRARIES_RELEASE}
    ${OPENAL_LIBRARIES_RELEASE}
    ${OGG_LIBRARIES_RELEASE}
    ${VORBIS_LIBRARIES_RELEASE}
    ${THEORA_LIBRARIES_RELEASE}
)

### Dusk library

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include
                    ${CMAKE_BINARY_DIR}/include)

# Allow imgui to #include "imgui.h" and stb sources
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/imgui
                    ${CMAKE_SOURCE_DIR}/include/stb)

# Disable imconfig.h, stb implementation
ADD_DEFINITIONS(
    -DIMGUI_DISABLE_INCLUDE_IMCONFIG_H
    -DIMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
    -DIMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
)

SET(Dusk_OUT Dusk)

SET(Dusk_CONFIG_IN include/dusk/Config.hpp.in)
SET(Dusk_CONFIG    ${CMAKE_BINARY_DIR}/include/dusk/Config.hpp)

FILE(GLOB_RECURSE Dusk_ASSETS RELATIVE ${CMAKE_SOURCE_DIR} "assets/*")

SET(Dusk_INCLUDES
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
    include/dusk/asset/ModelShader.hpp
    #include/dusk/asset/Sound.hpp
    include/dusk/asset/Texture.hpp
    #include/dusk/asset/Video.hpp
    include/dusk/asset/loaders/MeshLoaderFbx.hpp
    include/dusk/asset/loaders/MeshLoaderObj.hpp
    include/dusk/scene/Actor.hpp
    include/dusk/scene/Camera.hpp
    include/dusk/scene/Component.hpp
    include/dusk/scene/Model.hpp
    include/dusk/scene/Scene.hpp
    #include/dusk/scene/UI.hpp

    # imgui
    include/imgui/imgui.h
    include/imgui/imgui_internal.h
    include/imgui/imgui_impl_sdl_gl3.h

    # glad
    include/glad/glad.h
    include/KHR/khrplatform.h

    # stb
    include/stb/stb_image.h
)

SET(Dusk_SOURCES
    src/dusk/Dusk.cpp
    src/dusk/core/App.cpp
    src/dusk/core/ScriptHost.cpp
    src/dusk/core/Util.cpp
    #src/dusk/asset/Font.cpp
    src/dusk/asset/Material.cpp
    src/dusk/asset/Mesh.cpp
    src/dusk/asset/Shader.cpp
    src/dusk/asset/ModelShader.cpp
    #src/dusk/asset/Sound.cpp
    src/dusk/asset/Texture.cpp
    #src/dusk/asset/Video.cpp
    src/dusk/asset/loaders/MeshLoaderFbx.cpp
    src/dusk/asset/loaders/MeshLoaderObj.cpp
    src/dusk/scene/Actor.cpp
    src/dusk/scene/Camera.cpp
    src/dusk/scene/Component.cpp
    src/dusk/scene/Model.cpp
    src/dusk/scene/Scene.cpp
    #src/dusk/scene/UI.cpp

    # imgui
    src/imgui/imgui.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_demo.cpp
    src/imgui/imgui_impl_sdl_gl3.cpp

    # glad
    src/glad/glad.c
)

CONFIGURE_FILE(${Dusk_CONFIG_IN} ${Dusk_CONFIG})

ADD_LIBRARY(${Dusk_OUT}
    ${Dusk_CONFIG}
    ${Dusk_INCLUDES}
    ${Dusk_SOURCES}
)
ADD_DEPENDENCIES(${Dusk_OUT} depend)

SET_TARGET_PROPERTIES(
    ${Dusk_OUT} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

# Configuration-independant libraries
TARGET_LINK_LIBRARIES(
    ${Dusk_OUT}
    ${Dusk_LINK_LIBRARIES}
)

# Debug-only libraries
FOREACH(debug_lib IN ITEMS ${Dusk_LINK_LIBRARIES_DEBUG})
    TARGET_LINK_LIBRARIES(
        ${Dusk_OUT}
        debug ${debug_lib}
    )
ENDFOREACH()

# Release-only libraries
FOREACH(release_lib IN ITEMS ${Dusk_LINK_LIBRARIES_RELEASE})
    TARGET_LINK_LIBRARIES(
        ${Dusk_OUT}
        optimized ${release_lib}
    )
ENDFOREACH()

# Set IDE folders
FOREACH(file IN ITEMS ${Dusk_ASSETS} ${Dusk_INCLUDES} ${Dusk_SOURCES})
    GET_FILENAME_COMPONENT(file_path ${file} DIRECTORY)
    FILE(TO_NATIVE_PATH ${file_path} file_path)
    SOURCE_GROUP(${file_path} FILES ${file})
ENDFOREACH()

FILE(TO_NATIVE_PATH "include/dusk" config_path)
SOURCE_GROUP(${config_path} FILES ${Dusk_CONFIG})

# Copy assets to build directory
ADD_CUSTOM_TARGET(copy-assets ALL)
ADD_DEPENDENCIES(${Dusk_OUT} copy-assets)
ADD_CUSTOM_COMMAND(
    OUTPUT
    TARGET copy-assets PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E
        copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)

SET_TARGET_PROPERTIES(
    copy-assets
    PROPERTIES FOLDER "automation"
)

# Copy includes to build directory
ADD_CUSTOM_TARGET(copy-includes)
ADD_DEPENDENCIES(${Dusk_OUT} copy-includes)
FOREACH(inc ${Dusk_INCLUDES})
    ADD_CUSTOM_COMMAND(
        TARGET copy-includes PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E
            copy ${CMAKE_SOURCE_DIR}/${inc} ${CMAKE_BINARY_DIR}/${inc}
    )
ENDFOREACH()

SET_TARGET_PROPERTIES(
    copy-includes
    PROPERTIES FOLDER "automation"
)

### Example projects

ADD_SUBDIRECTORY(examples)

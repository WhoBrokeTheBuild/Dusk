CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)
CMAKE_POLICY(SET CMP0048 NEW)

PROJECT(Dusk VERSION 0.0.2)

INCLUDE("cmake/Compiler.cmake")

###
### Global configuration
###

# Allow for custom CMake modules
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Allow for custom organization of files in VisualStudio
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Get Git revision
INCLUDE(GetGitRevisionDescription)
GET_GIT_HEAD_REVISION(GIT_REFSPEC DUSK_REVISION)
STRING(SUBSTRING "${DUSK_REVISION}" 0 12 DUSK_REVISION)

SET(ASSET_PATH "${CMAKE_SOURCE_DIR}/assets")

###
### Projects
###

# Dependencies to be built with the project
ADD_SUBDIRECTORY(depend)

# Engine library
ADD_SUBDIRECTORY(engine)

# Editor library
ADD_SUBDIRECTORY(editor)

# Preprocessor
ADD_SUBDIRECTORY(preprocessor)

# Example projects
#ADD_SUBDIRECTORY(examples)

STRING(REPLACE "\\" "/" DUSK_PATH ${CMAKE_SOURCE_DIR})
STRING(REPLACE " " "\\ " DUSK_PATH ${CMAKE_SOURCE_DIR})

CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/cmake/Config.cmake.in"
    "${CMAKE_SOURCE_DIR}/cmake/Config.cmake"
    @ONLY
)

IF(WIN32)
    CONFIGURE_FILE(
        "${CMAKE_SOURCE_DIR}/cmake/DuskPath.reg.in"
        "${CMAKE_SOURCE_DIR}/cmake/DuskPath.reg"
        @ONLY
    )
    CONFIGURE_FILE(
        "${CMAKE_SOURCE_DIR}/cmake/NewDuskProject.reg.in"
        "${CMAKE_SOURCE_DIR}/cmake/NewDuskProject.reg"
        @ONLY
    )
ENDIF()

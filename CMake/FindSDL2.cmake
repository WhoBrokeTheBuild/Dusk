# FindSDL2.cmake
#
# Finds the Simple Direct Media Library version 2
#
# This will define the following variables
#
#   SDL2_FOUND
#   SDL2_INCLUDE_DIR
#   SDL2_LIBRARIES
#   SDL2_RUNTIME_DIR
#
# and the following imported targets
#
#   SDL2::SDL2
#
# The following variables can be set as arguments
#
#   SDL2_ROOT_DIR
#

FIND_PACKAGE(PkgConfig QUIET)
IF(PKG_CONFIG_FOUND)
    PKG_CHECK_MODULES(_sdl2_PC QUIET sdl2)
ENDIF()

FIND_PACKAGE(sdl2 QUIET CONFIG)

SET(SDL2_INCLUDE_DIR ${_sdl2_PC_INCLUDE_DIRS})
FIND_PATH(
    SDL2_INCLUDE_DIR
    NAMES SDL.h
    PATHS 
        ${SDL2_ROOT_DIR}
        ${_sdl2_PC_INCLUDE_DIRS}
        ${SDL2_PREFIX}
    PATH_SUFFIXES 
        "SDL2"
        "include/SDL2"
)

FIND_PATH(
    SDL2_RUNTIME_DIR
    NAMES SDL2.dll
    PATHS 
        ${SDL2_ROOT_DIR}
        ${_sdl2_PC_LIBRARY_DIRS}
        ${SDL2_PREFIX}
    PATH_SUFFIXES 
        "lib" 
        "bin"
)

IF(NOT SDL2_RUNTIME_DIR)
    SET(SDL2_RUNTIME_DIR "")
ENDIF()

FIND_LIBRARY(
    SDL2_LIBRARY
    NAMES SDL2
    PATHS 
        ${SDL2_ROOT_DIR}
        ${_sdl2_PC_LIBRARY_DIRS}
        ${SDL2_PREFIX}
    PATH_SUFFIXES "lib"
)

FIND_LIBRARY(
    SDL2_MAIN_LIBRARY
    NAMES SDL2main
    PATHS
        ${SDL2_ROOT_DIR}
        ${_sdl2_PC_LIBRARY_DIRS}
        ${SDL2_PREFIX}
    PATH_SUFFIXES "lib"
)

IF(SDL2_MAIN_LIBRARY AND SDL2_LIBRARY)
    LIST(APPEND SDL2_LIBRARY "${SDL2_MAIN_LIBRARY}")
ENDIF()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    SDL2
    REQUIRED_VARS 
        SDL2_INCLUDE_DIR
        SDL2_LIBRARY 
)

IF(SDL2_FOUND)
    ADD_LIBRARY(SDL2::SDL2 INTERFACE IMPORTED)
    SET_TARGET_PROPERTIES(
        SDL2::SDL2 PROPERTIES
        INTERFACE_LINK_LIBRARIES      "${SDL2_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}"
    )
ENDIF()

# FindSDL2.cmake
#
# Finds the OpenGL Mathematics library
#
# This will define the following variables
#
#   SDL2_FOUND
#   SDL2_INCLUDE_DIR
#   SDL2_LIBRARIES
#
# and the following imported targets
#
#   SDL2::SDL2
#
# The following variables can be set as arguments
#
#   SDL2_ROOT_DIR
#

FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(_SDL2_PC QUIET sdl2)

FIND_PATH(
    SDL2_INCLUDE_DIR
    NAMES SDL.h
    PATHS ${_SDL2_PC_INCLUDE_DIRS} ${SDL2_ROOT_DIR}
    PATH_SUFFIXES include/SDL2
)

FIND_LIBRARY(
    SDL2_LIBRARY
    NAMES SDL2
    PATHS ${_SDL2_PC_INCLUDE_DIRS} ${SDL2_ROOT_DIR}
    PATH_SUFFIXES lib
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    SDL2
    REQUIRED_VARS 
        SDL2_INCLUDE_DIR
        SDL2_LIBRARY 
)

ADD_LIBRARY(SDL2::SDL2 INTERFACE IMPORTED)
SET_TARGET_PROPERTIES(
    SDL2::SDL2 PROPERTIES
    INTERFACE_LINK_LIBRARIES "${SDL2_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}"
)


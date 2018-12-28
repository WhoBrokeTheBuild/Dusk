# FindAssimp.cmake
#
# Finds the OpenGL Mathematics library
#
# This will define the following variables
#
#   Assimp_FOUND
#   Assimp_INCLUDE_DIR
#   Assimp_LIBRARIES
#
# and the following imported targets
#
#   Assimp::Assimp
#
# The following variables can be set as arguments
#
#   Assimp_ROOT_DIR
#

FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(_ASSIMP_PC QUIET assimp)

FIND_PATH(
    Assimp_INCLUDE_DIR
    NAMES assimp/config.h
    PATHS ${_ASSIMP_PC_INCLUDE_DIRS} ${Assimp_ROOT_DIR}
    PATH_SUFFIXES include
)

FIND_LIBRARY(
    Assimp_LIBRARY
    NAMES assimp
    PATHS ${_ASSIMP_PC_INCLUDE_DIRS} ${Assimp_ROOT_DIR}
    PATH_SUFFIXES lib
)

# TODO Zlib

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    Assimp
    REQUIRED_VARS 
        Assimp_INCLUDE_DIR
        Assimp_LIBRARY
)

ADD_LIBRARY(Assimp::Assimp INTERFACE IMPORTED)
SET_TARGET_PROPERTIES(
    Assimp::Assimp PROPERTIES
    INTERFACE_LINK_LIBRARIES "${Assimp_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${Assimp_INCLUDE_DIR}"
)


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

IF(MSVC70 OR MSVC71)
    SET(MSVC_PREFIX "vc70")
ELSEIF(MSVC80)
    SET(MSVC_PREFIX "vc80")
ELSEIF(MSVC90)
    SET(MSVC_PREFIX "vc90")
ELSEIF(MSVC10)
    SET(MSVC_PREFIX "vc100")
ELSEIF(MSVC11)
    SET(MSVC_PREFIX "vc110")
ELSEIF(MSVC12)
    SET(MSVC_PREFIX "vc120")
ELSEIF(MSVC14)
    SET(MSVC_PREFIX "vc140")
ELSE()
    SET(MSVC_PREFIX "vc150")
ENDIF()

FIND_PATH(
    Assimp_INCLUDE_DIR
    NAMES assimp/config.h
    PATHS ${_ASSIMP_PC_INCLUDE_DIRS} ${Assimp_ROOT_DIR}
    PATH_SUFFIXES include
)

FIND_LIBRARY(
    Assimp_LIBRARY
    NAMES assimp assimp-${MSVC_PREFIX}-mt
    PATHS ${_ASSIMP_PC_INCLUDE_DIRS} ${Assimp_ROOT_DIR}
    PATH_SUFFIXES lib
)

FIND_LIBRARY(
    IrrXML_LIBRARY
    NAMES IrrXML
    PATHS ${_ASSIMP_PC_INCLUDE_DIRS} ${Assimp_ROOT_DIR}
    PATH_SUFFIXES lib
)

IF(IrrXML_LIBRARY AND Assimp_LIBRARY)
    LIST(APPEND Assimp_LIBRARY "${IrrXML_LIBRARY}")
ENDIF()

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


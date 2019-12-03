# FindOpenAL.cmake
#
# Finds the Open Audio Library
#
# This will define the following variables
#
#   OpenAL_FOUND
#   OpenAL_INCLUDE_DIR
#   OpenAL_LIBRARIES
#   OpenAL_RUNTIME_DIR
#
# and the following imported targets
#
#   OpenAL::OpenAL
#
# The following variables can be set as arguments
#
#   OpenAL_ROOT_DIR
#

FIND_PACKAGE(PkgConfig QUIET)
PKG_CHECK_MODULES(_OpenAL_PC QUIET openal)

SET(OpenAL_INCLUDE_DIR ${_OpenAL_PC_INCLUDE_DIRS})
FIND_PATH(
    OpenAL_INCLUDE_DIR
    NAMES AL/al.h
    PATHS ${OpenAL_ROOT_DIR}
    PATH_SUFFIXES include
)

FIND_PATH(
    OpenAL_RUNTIME_DIR
    NAMES OpenAL32.dll
    PATHS ${OpenAL_ROOT_DIR}
    PATH_SUFFIXES bin
)

IF(NOT OpenAL_RUNTIME_DIR)
    SET(OpenAL_RUNTIME_DIR "")
ENDIF()

SET(OpenAL_LIBRARY ${_OpenAL_PC_LIBRARIES})
FIND_LIBRARY(
    OpenAL_LIBRARY
    NAMES OpenAL32 openal
    PATHS ${OpenAL_ROOT_DIR} 
    PATH_SUFFIXES lib
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    OpenAL
    REQUIRED_VARS 
        OpenAL_INCLUDE_DIR
        OpenAL_LIBRARY 
)

ADD_LIBRARY(OpenAL::OpenAL INTERFACE IMPORTED)
SET_TARGET_PROPERTIES(
    OpenAL::OpenAL PROPERTIES
    INTERFACE_LINK_LIBRARIES "${OpenAL_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${OpenAL_INCLUDE_DIR}"
)


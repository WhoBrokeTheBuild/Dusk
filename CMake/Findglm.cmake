# Findglm.cmake
#
# Finds the OpenGL Mathematics library
#
# This will define the following variables
#
#   glm_FOUND
#   glm_INCLUDE_DIR
#
# and the following imported targets
#
#   glm::glm
#
# The following variables can be set as arguments
#
#   glm_ROOT_DIR
#

FIND_PACKAGE(PkgConfig QUIET)
IF(PKG_CONFIG_FOUND)
    PKG_CHECK_MODULES(_glm_PC QUIET glm)
ENDIF()

FIND_PATH(
    glm_INCLUDE_DIR
    NAMES glm/glm.hpp
    PATHS 
        ${glm_ROOT_DIR} 
        ${_glm_PC_INCLUDE_DIRS} 
    PATH_SUFFIXES "include"
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    glm
    REQUIRED_VARS 
        glm_INCLUDE_DIR
)

IF(glm_FOUND)
    ADD_LIBRARY(glm::glm INTERFACE IMPORTED)
    SET_TARGET_PROPERTIES(
        glm::glm PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${glm_INCLUDE_DIR}"
    )
ENDIF()

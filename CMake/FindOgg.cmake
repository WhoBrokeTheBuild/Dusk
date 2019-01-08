# FindOgg.cmake
#
# Finds Xiph.org's Ogg, Vorbis, and Theora Libraries
#
# This will define the following variables
#
#   Ogg_FOUND
#   Ogg_INCLUDE_DIR
#   Ogg_LIBRARY
#   Ogg_RUNTIME_DIR
#   Vorbis_FOUND
#   Vorbis_INCLUDE_DIR
#   Vorbis_LIBRARY
#   VorbisFile_LIBRARY
#   Vorbis_RUNTIME_DIR
#   Theora_FOUND
#   Theora_INCLUDE_DIR
#   Theora_LIBRARY
#   Theora_RUNTIME_DIR
#
# and the following imported targets
#
#   Ogg::Ogg
#
# The following variables can be set as arguments
#
#   Ogg_ROOT_DIR
#

FIND_PACKAGE(PkgConfig QUIET)
PKG_CHECK_MODULES(_Ogg_PC QUIET ogg)
PKG_CHECK_MODULES(_Vorbis_PC QUIET vorbis)
PKG_CHECK_MODULES(_VorbisFile_PC QUIET vorbisfile)
PKG_CHECK_MODULES(_Theora_PC QUIET theora)

SET(Ogg_INCLUDE_DIR ${_Ogg_PC_INCLUDE_DIRS})
FIND_PATH(
    Ogg_INCLUDE_DIR
    NAMES ogg/ogg.h
    PATHS ${Ogg_ROOT_DIR}
    PATH_SUFFIXES include
)

SET(Vorbis_INCLUDE_DIR ${_Vorbis_PC_INCLUDE_DIRS})
FIND_PATH(
    Vorbis_INCLUDE_DIR
    NAMES vorbis/vorbisfile.h
    PATHS ${Vorbis_ROOT_DIR}
    PATH_SUFFIXES include
)

SET(Theora_INCLUDE_DIR ${_Theora_PC_INCLUDE_DIRS})
FIND_PATH(
    Theora_INCLUDE_DIR
    NAMES theora/theora.h
    PATHS ${Theora_ROOT_DIR}
    PATH_SUFFIXES include
)

SET(Ogg_LIBRARY ${_Ogg_PC_LIBRARIES})
FIND_LIBRARY(
    Ogg_LIBRARY
    NAMES ogg libogg_static
    PATHS ${Ogg_ROOT_DIR}
    PATH_SUFFIXES lib
)

SET(Vorbis_LIBRARY ${_Vorbis_PC_LIBRARIES})
FIND_LIBRARY(
    Vorbis_LIBRARY
    NAMES vorbis libvorbis_static
    PATHS ${Vorbis_ROOT_DIR}
    PATH_SUFFIXES lib
)

SET(VorbisFile_LIBRARY ${_VorbisFile_PC_LIBRARIES})
FIND_LIBRARY(
    VorbisFile_LIBRARY
    NAMES vorbisfile libvorbisfile_static
    PATHS ${Vorbis_ROOT_DIR}
    PATH_SUFFIXES lib
)

SET(Theora_LIBRARY ${_Theora_PC_LIBRARIES})
FIND_LIBRARY(
    Theora_LIBRARY
    NAMES theora libtheora_static
    PATHS ${Theora_ROOT_DIR}
    PATH_SUFFIXES lib
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    Ogg
    REQUIRED_VARS 
        Ogg_INCLUDE_DIR
        Ogg_LIBRARY 
)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    Vorbis
    REQUIRED_VARS 
        Vorbis_INCLUDE_DIR
        Vorbis_LIBRARY 
        VorbisFile_LIBRARY 
)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    Theora
    REQUIRED_VARS 
        Theora_INCLUDE_DIR
        Theora_LIBRARY 
)

ADD_LIBRARY(Ogg::Ogg INTERFACE IMPORTED)
SET_TARGET_PROPERTIES(
    Ogg::Ogg PROPERTIES
    INTERFACE_LINK_LIBRARIES "${Ogg_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${Ogg_INCLUDE_DIR}"
)

ADD_LIBRARY(Ogg::Vorbis INTERFACE IMPORTED)
SET_TARGET_PROPERTIES(
    Ogg::Vorbis PROPERTIES
    INTERFACE_LINK_LIBRARIES "${Vorbis_LIBRARY};${VorbisFile_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${Vorbis_INCLUDE_DIR}"
)

ADD_LIBRARY(Ogg::Theora INTERFACE IMPORTED)
SET_TARGET_PROPERTIES(
    Ogg::Theora PROPERTIES
    INTERFACE_LINK_LIBRARIES "${Theora_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${Theora_INCLUDE_DIR}"
)

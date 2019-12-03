# FindOgg.cmake
#
# Finds Xiph.org's Ogg, Vorbis, and Theora Libraries
#
# The following components are supported
#
#   Vorbis
#   Theora
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

SET(Ogg_INCLUDE_DIR ${_Ogg_PC_INCLUDE_DIRS})
FIND_PATH(
    Ogg_INCLUDE_DIR
    NAMES ogg/ogg.h
    PATHS ${Ogg_ROOT_DIR}
    PATH_SUFFIXES include
)

SET(Ogg_LIBRARY ${_Ogg_PC_LIBRARIES})
FIND_LIBRARY(
    Ogg_LIBRARY
    NAMES ogg libogg_static
    PATHS ${Ogg_ROOT_DIR}
    PATH_SUFFIXES lib
)

IF(Ogg_FIND_REQUIRED_Vorbis OR Ogg_FIND_REQUIRED_Theora)
    PKG_CHECK_MODULES(_Vorbis_PC QUIET vorbis)
    PKG_CHECK_MODULES(_VorbisFile_PC QUIET vorbisfile)

    SET(Vorbis_INCLUDE_DIR ${_Vorbis_PC_INCLUDE_DIRS})
    FIND_PATH(
        Vorbis_INCLUDE_DIR
        NAMES vorbis/vorbisfile.h
        PATHS ${Vorbis_ROOT_DIR}
        PATH_SUFFIXES include
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

    IF(Vorbis_INCLUDE_DIR AND Vorbis_LIBRARY AND VorbisFile_LIBRARY)
        SET(Ogg_Vorbis_FOUND TRUE)
    ENDIF()
ENDIF()

IF(Ogg_FIND_REQUIRED_Theora)
    PKG_CHECK_MODULES(_Theora_PC QUIET theora)

    SET(Theora_INCLUDE_DIR ${_Theora_PC_INCLUDE_DIRS})
    FIND_PATH(
        Theora_INCLUDE_DIR
        NAMES theora/theora.h
        PATHS ${Theora_ROOT_DIR}
        PATH_SUFFIXES include
    )

    SET(Theora_LIBRARY ${_Theora_PC_LIBRARIES})
    FIND_LIBRARY(
        Theora_LIBRARY
        NAMES theora libtheora_static
        PATHS ${Theora_ROOT_DIR}
        PATH_SUFFIXES lib
    )

    IF(Theora_INCLUDE_DIR AND Theora_LIBRARY)
        SET(Ogg_Theora_FOUND TRUE)
    ENDIF()
ENDIF()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    Ogg
    HANDLE_COMPONENTS
    REQUIRED_VARS 
        Ogg_INCLUDE_DIR
        Ogg_LIBRARY
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

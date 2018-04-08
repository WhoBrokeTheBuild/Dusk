
ADD_CUSTOM_TARGET(depend)

SET(SHARED_EXT "so")
IF(APPLE)
    SET(SHARED_EXT "dylib")
ENDIF()

### opengl
FIND_PACKAGE(OpenGL REQUIRED)

### glm
MESSAGE("Adding dependency: glm")

SET(GLM_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/depend/glm")

ADD_CUSTOM_TARGET(glm)
ADD_DEPENDENCIES(depend glm)

### nlohmann_json
MESSAGE("Adding dependency: json")

SET(JSON_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/depend/json/include")

ADD_CUSTOM_TARGET(json)
ADD_DEPENDENCIES(depend json)

### gsl
MESSAGE("Adding dependency: gsl")

SET(GSL_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/depend/gsl/include")

ADD_CUSTOM_TARGET(gsl)
ADD_DEPENDENCIES(depend gsl)

### lua
MESSAGE("Adding dependency: lua")

SET(LUA_OPTIONS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
)

IF(MSVC)
    SET(LUA_OPTIONS
        -DLUA_BUILD_AS_DLL=OFF
        -DBUILD_SHARED_LIBS=OFF
    )
ENDIF()

SET(LUA_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/depend/lua/src"
    "${CMAKE_BINARY_DIR}/depend/lua"

)

FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/depend/lua)
EXECUTE_PROCESS(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}"
        ${CMAKE_SOURCE_DIR}/depend/lua ${LUA_OPTIONS}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/lua
)

IF (MSVC)
    SET(LUA_LIBRARIES_DEBUG "${CMAKE_BINARY_DIR}/depend/lua/Debug/lua.lib")
    SET(LUA_LIBRARIES_DEBUG "${LUA_LIBRARIES_DEBUG}")
    SET(LUA_LIBRARIES_RELEASE "${CMAKE_BINARY_DIR}/depend/lua/Release/lua.lib")
    SET(LUA_LIBRARIES_RELEASE "${LUA_LIBRARIES_RELEASE}")

    ADD_CUSTOM_COMMAND(
        OUTPUT ${LUA_LIBRARIES_DEBUG}
        COMMAND ${CMAKE_COMMAND} --build . --config Debug --target liblua
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/lua
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${LUA_LIBRARIES_RELEASE}
        COMMAND ${CMAKE_COMMAND} --build . --config Release --target liblua
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/lua
    )
    ADD_CUSTOM_TARGET(lua DEPENDS ${LUA_LIBRARIES_DEBUG} ${LUA_LIBRARIES_RELEASE})
ELSE()
    SET(LUA_LIBRARIES "${CMAKE_BINARY_DIR}/depend/lua/liblua.${SHARED_EXT}")
    SET(LUA_LIBRARIES "${LUA_LIBRARIES}")

    ADD_CUSTOM_COMMAND(
        OUTPUT ${LUA_LIBRARIES}
        COMMAND ${CMAKE_COMMAND} --build . --target liblua
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/lua
    )
    ADD_CUSTOM_TARGET(lua DEPENDS ${LUA_LIBRARIES})
ENDIF()
ADD_DEPENDENCIES(depend lua)

### zlib
MESSAGE("Adding dependency: zlib")

SET(ZLIB_OPTIONS
)

SET(ZLIB_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/depend/zlib"
    "${CMAKE_BINARY_DIR}/depend/zlib"

)

FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/depend/zlib)
EXECUTE_PROCESS(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}"
        ${CMAKE_SOURCE_DIR}/depend/zlib ${ZLIB_OPTIONS}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/zlib
)

IF (MSVC)
    SET(ZLIB_LIBRARIES_DEBUG "${CMAKE_BINARY_DIR}/depend/zlib/Debug/zlibd.lib")
    SET(ZLIB_LIBRARIES_DEBUG "${ZLIB_LIBRARIES_DEBUG}")
    SET(ZLIB_LIBRARIES_RELEASE "${CMAKE_BINARY_DIR}/depend/zlib/Release/zlib.lib")
    SET(ZLIB_LIBRARIES_RELEASE "${ZLIB_LIBRARIES_RELEASE}")

    ADD_CUSTOM_COMMAND(
        OUTPUT ${ZLIB_LIBRARIES_DEBUG}
        COMMAND ${CMAKE_COMMAND} --build . --config Debug --target zlib
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/zlib
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${ZLIB_LIBRARIES_RELEASE}
        COMMAND ${CMAKE_COMMAND} --build . --config Release --target zlib
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/zlib
    )
    ADD_CUSTOM_TARGET(zlib DEPENDS ${ZLIB_LIBRARIES_DEBUG} ${ZLIB_LIBRARIES_RELEASE})
ELSE()
    SET(ZLIB_LIBRARIES "${CMAKE_BINARY_DIR}/depend/zlib/libz.${SHARED_EXT}")
    SET(ZLIB_LIBRARIES "${ZLIB_LIBRARIES}")

    ADD_CUSTOM_COMMAND(
        OUTPUT ${ZLIB_LIBRARIES}
        COMMAND ${CMAKE_COMMAND} --build . --target zlib
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/zlib
    )
    ADD_CUSTOM_TARGET(zlib DEPENDS ${ZLIB_LIBRARIES})
ENDIF()
ADD_DEPENDENCIES(depend zlib)

### SDL2
MESSAGE("Adding dependency: SDL2")

SET(SDL2_OPTIONS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
)

IF(NOT MSVC)
    SET(SDL2_OPTIONS ${SDL2_OPTIONS} -DBUILD_SHARED_LIBS=ON)
ENDIF()

SET(SDL2_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/depend/SDL2/include"
    "${CMAKE_BINARY_DIR}/depend/SDL2/include"

)

FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/depend/SDL2)
EXECUTE_PROCESS(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}"
        ${CMAKE_SOURCE_DIR}/depend/SDL2 ${SDL2_OPTIONS}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/SDL2
)

IF (MSVC)
    SET(SDL2_LIBRARIES_DEBUG "${CMAKE_BINARY_DIR}/depend/SDL2/Debug/libSDL2.lib")
    SET(SDL2_LIBRARIES_DEBUG "${SDL2_LIBRARIES_DEBUG}")
    SET(SDL2_LIBRARIES_RELEASE "${CMAKE_BINARY_DIR}/depend/SDL2/Release/libSDL2.lib")
    SET(SDL2_LIBRARIES_RELEASE "${SDL2_LIBRARIES_RELEASE}")

    ADD_CUSTOM_COMMAND(
        OUTPUT ${SDL2_LIBRARIES_DEBUG}
        COMMAND ${CMAKE_COMMAND} --build . --config Debug --target SDL2-static
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/sdl2
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${SDL2_LIBRARIES_RELEASE}
        COMMAND ${CMAKE_COMMAND} --build . --config Release --target SDL2-static
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/sdl2
    )
    ADD_CUSTOM_TARGET(sdl2 DEPENDS ${SDL2_LIBRARIES_DEBUG} ${SDL2_LIBRARIES_RELEASE})
ELSE()
    STRING(TOLOWER "${CMAKE_BUILD_TYPE}" build_type)
    IF (build_type STREQUAL "debug")
        SET(SDL2_LIBRARIES "${CMAKE_BINARY_DIR}/depend/SDL2/libSDL2-2.0d.${SHARED_EXT}")
    ELSE()
        SET(SDL2_LIBRARIES "${CMAKE_BINARY_DIR}/depend/SDL2/libSDL2-2.0.${SHARED_EXT}")
    ENDIF()
    SET(SDL2_LIBRARIES "${SDL2_LIBRARIES}")

    ADD_CUSTOM_COMMAND(
        OUTPUT ${SDL2_LIBRARIES}
        COMMAND ${CMAKE_COMMAND} --build . --target SDL2
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/SDL2
    )
    ADD_CUSTOM_TARGET(sdl2 DEPENDS ${SDL2_LIBRARIES})
ENDIF()
ADD_DEPENDENCIES(depend sdl2)

### OpenAL
MESSAGE("Adding dependency: openal")

SET(OPENAL_OPTIONS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DALSOFT_UTILS=OFF
    -DALSOFT_EXAMPLES=OFF
    -DALSOFT_TESTS=OFF
    -DALSOFT_INSTALL=OFF
)

SET(OPENAL_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/depend/openal/include"
    "${CMAKE_BINARY_DIR}/depend/openal"
)

FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/depend/openal)
EXECUTE_PROCESS(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}"
        ${CMAKE_SOURCE_DIR}/depend/openal ${OPENAL_OPTIONS}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/openal
)

IF (MSVC)
    SET(OPENAL_LIBRARIES_DEBUG "${CMAKE_BINARY_DIR}/depend/openal/Debug/OpenAL32.lib")
    SET(OPENAL_LIBRARIES_DEBUG "${OPENAL_LIBRARIES_DEBUG}")
    SET(OPENAL_LIBRARIES_RELEASE "${CMAKE_BINARY_DIR}/depend/openal/Release/OpenAL32.lib")
    SET(OPENAL_LIBRARIES_RELEASE "${OPENAL_LIBRARIES_RELEASE}")

    ADD_CUSTOM_COMMAND(
        OUTPUT ${OPENAL_LIBRARIES_DEBUG}
        COMMAND ${CMAKE_COMMAND} --build . --config Debug --target openal
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/openal
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${OPENAL_LIBRARIES_RELEASE}
        COMMAND ${CMAKE_COMMAND} --build . --config Release --target openal
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/openal
    )
    ADD_CUSTOM_TARGET(openal DEPENDS ${OPENAL_LIBRARIES_DEBUG} ${OPENAL_LIBRARIES_RELEASE})
ELSE()
    SET(OPENAL_LIBRARIES "${CMAKE_BINARY_DIR}/depend/openal/libopenal.${SHARED_EXT}")
    SET(OPENAL_LIBRARIES "${OPENAL_LIBRARIES}")

    ADD_CUSTOM_COMMAND(
        OUTPUT ${OPENAL_LIBRARIES}
        COMMAND ${CMAKE_COMMAND} --build . --target OpenAL
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/openal
    )
    ADD_CUSTOM_TARGET(openal DEPENDS ${OPENAL_LIBRARIES})
ENDIF()
ADD_DEPENDENCIES(depend openal)

#[[

### Ogg
MESSAGE("Adding dependency: ogg")

SET(OGG_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/depend/libogg/include"
)

IF (MSVC)
    MESSAGE(STATUS "Using libogg_static.sln")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(OGG_LIBRARIES_DEBUG "${CMAKE_SOURCE_DIR}/depend/libogg/win32/VS2010/x64/Debug/libogg_static.lib")
        SET(OGG_LIBRARIES_RELEASE "${CMAKE_SOURCE_DIR}/depend/libogg/win32/VS2010/x64/Release/libogg_static.lib")
        SET(OGG_PLATFORM "x64")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        SET(OGG_LIBRARIES_DEBUG "${CMAKE_SOURCE_DIR}/depend/libogg/win32/VS2010/Win32/Debug/libogg_static.lib")
        SET(OGG_LIBRARIES_RELEASE "${CMAKE_SOURCE_DIR}/depend/libogg/win32/VS2010/Win32/Release/libogg_static.lib")
        SET(OGG_PLATFORM "Win32")
    endif()
    SET(OGG_LIBRARIES_DEBUG "${OGG_LIBRARIES_DEBUG}")
    SET(OGG_LIBRARIES_RELEASE "${OGG_LIBRARIES_RELEASE}")

    FILE(REMOVE_RECURSE
        ${CMAKE_SOURCE_DIR}/depend/libogg/win32/VS2010/UpgradeLog.htm
        ${CMAKE_SOURCE_DIR}/depend/libogg/win32/VS2010/Backup
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${OGG_LIBRARIES_DEBUG}
        COMMAND ${CMAKE_VS_DEVENV_COMMAND} /Upgrade win32/VS2010/libogg_static.sln;
        COMMAND ${CMAKE_VS_MSBUILD_COMMAND} win32/VS2010/libogg_static.sln /t:Build /p:WarningLevel=0 /p:Configuration=Debug /p:Platform=${OGG_PLATFORM}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/depend/libogg
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${OGG_LIBRARIES_RELEASE}
        COMMAND ${CMAKE_VS_DEVENV_COMMAND} /Upgrade win32/VS2010/libogg_static.sln;
        COMMAND ${CMAKE_VS_MSBUILD_COMMAND} win32/VS2010/libogg_static.sln /t:Build /p:WarningLevel=0 /p:Configuration=Release /p:Platform=${OGG_PLATFORM}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/depend/libogg
    )
    ADD_CUSTOM_TARGET(ogg DEPENDS ${OGG_LIBRARIES_DEBUG} ${OGG_LIBRARIES_RELEASE})
ELSE()
    MESSAGE(STATUS "Using autogen.sh")
    SET(OGG_LIBRARIES "${CMAKE_SOURCE_DIR}/depend/libogg/src/.libs/libogg.${SHARED_EXT}")
    SET(OGG_LIBRARIES "${OGG_LIBRARIES}")

    SET(OGG_AUTOCONF_STAMP "${CMAKE_SOURCE_DIR}/depend/libogg/stamp-h1")
    ADD_CUSTOM_COMMAND(
        OUTPUT ${OGG_AUTOCONF_STAMP}
        COMMAND ./autogen.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/depend/libogg
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${OGG_LIBRARIES}
        MAIN_DEPENDENCY ${OGG_AUTOCONF_STAMP}
        COMMAND make
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/depend/libogg
    )
    ADD_CUSTOM_TARGET(ogg DEPENDS ${OGG_LIBRARIES})
ENDIF()
ADD_DEPENDENCIES(depend ogg)

### Vorbis
MESSAGE("Adding dependency: vorbis")

SET(VORBIS_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/depend/libvorbis/include"
)

IF (MSVC)
    MESSAGE(STATUS "Using vorbis_static.sln")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(VORBIS_LIBRARIES_DEBUG
            "${CMAKE_SOURCE_DIR}/depend/libvorbis/win32/VS2010/x64/Debug/libvorbis_static.lib"
            "${CMAKE_SOURCE_DIR}/depend/libvorbis/win32/VS2010/x64/Debug/libvorbisfile_static.lib")
        SET(VORBIS_LIBRARIES_RELEASE
            "${CMAKE_SOURCE_DIR}/depend/libvorbis/win32/VS2010/x64/Release/libvorbis_static.lib"
            "${CMAKE_SOURCE_DIR}/depend/libvorbis/win32/VS2010/x64/Release/libvorbisfile_static.lib")
        SET(VORBIS_PLATFORM "x64")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        SET(VORBIS_LIBRARIES_DEBUG
            "${CMAKE_SOURCE_DIR}/depend/libvorbis/win32/VS2010/Win32/Debug/libvorbis_static.lib"
            "${CMAKE_SOURCE_DIR}/depend/libvorbis/win32/VS2010/Win32/Debug/libvorbisfile_static.lib")
        SET(VORBIS_LIBRARIES_RELEASE
            "${CMAKE_SOURCE_DIR}/depend/libvorbis/win32/VS2010/Win32/Release/libvorbis_static.lib"
            "${CMAKE_SOURCE_DIR}/depend/libvorbis/win32/VS2010/Win32/Release/libvorbisfile_static.lib")
        SET(VORBIS_PLATFORM "Win32")
    endif()
    SET(VORBIS_LIBRARIES_DEBUG "${VORBIS_LIBRARIES_DEBUG}")
    SET(VORBIS_LIBRARIES_RELEASE "${VORBIS_LIBRARIES_RELEASE}")

    FILE(REMOVE_RECURSE
        ${CMAKE_SOURCE_DIR}/depend/libvorbis/win32/VS2010/UpgradeLog.htm
        ${CMAKE_SOURCE_DIR}/depend/libvorbis/win32/VS2010/Backup
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${VORBIS_LIBRARIES_DEBUG}
        COMMAND ${CMAKE_VS_DEVENV_COMMAND} /Upgrade win32/VS2010/vorbis_static.sln
        COMMAND ${CMAKE_VS_MSBUILD_COMMAND} win32/VS2010/vorbis_static.sln /t:Build /p:WarningLevel=0 /p:Configuration=Debug /p:Platform=${VORBIS_PLATFORM}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/depend/libvorbis
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${VORBIS_LIBRARIES_RELEASE}
        COMMAND ${CMAKE_VS_DEVENV_COMMAND} /Upgrade win32/VS2010/vorbis_static.sln;
        COMMAND ${CMAKE_VS_MSBUILD_COMMAND} win32/VS2010/vorbis_static.sln /t:Build /p:WarningLevel=0 /p:Configuration=Release /p:Platform=${VORBIS_PLATFORM}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/depend/libvorbis
    )
    ADD_CUSTOM_TARGET(vorbis DEPENDS ${VORBIS_LIBRARIES_DEBUG} ${VORBIS_LIBRARIES_RELEASE})
ELSE()
    MESSAGE(STATUS "Using autogen.sh")
    SET(VORBIS_LIBRARIES "${CMAKE_SOURCE_DIR}/depend/libvorbis/lib/.libs/libvorbis.${SHARED_EXT}")
    SET(VORBIS_LIBRARIES "${VORBIS_LIBRARIES}")

    SET(VORBIS_AUTOCONF_STAMP "${CMAKE_SOURCE_DIR}/depend/libvorbis/stamp-h1")
    ADD_CUSTOM_COMMAND(
        OUTPUT ${VORBIS_AUTOCONF_STAMP}
        COMMAND ${CMAKE_COMMAND} -E env
                CPPFLAGS="-I${CMAKE_SOURCE_DIR}/depend/libogg/include"
                LDFLAGS="-L${CMAKE_SOURCE_DIR}/depend/libogg/src/.libs"
                ./autogen.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/depend/libvorbis
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${VORBIS_LIBRARIES}
        MAIN_DEPENDENCY ${VORBIS_AUTOCONF_STAMP}
        COMMAND make
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/depend/libvorbis
    )
    ADD_CUSTOM_TARGET(vorbis DEPENDS ogg ${VORBIS_LIBRARIES})
ENDIF()
ADD_DEPENDENCIES(depend vorbis)

### Theora
MESSAGE("Adding dependency: theora")

SET(THEORA_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/depend/libtheora/include"
)

IF (MSVC)
    MESSAGE(STATUS "Using theora_static.sln")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(THEORA_LIBRARIES_DEBUG
            "${CMAKE_SOURCE_DIR}/depend/libtheora/win32/VS2010/x64/Debug/libtheora_static.lib"
            "${CMAKE_SOURCE_DIR}/depend/libtheora/win32/VS2010/x64/Debug/libtheorafile_static.lib")
        SET(THEORA_LIBRARIES_RELEASE
            "${CMAKE_SOURCE_DIR}/depend/libtheora/win32/VS2010/x64/Release/libtheora_static.lib"
            "${CMAKE_SOURCE_DIR}/depend/libtheora/win32/VS2010/x64/Release/libtheorafile_static.lib")
        SET(THEORA_PLATFORM "x64")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        SET(THEORA_LIBRARIES_DEBUG
            "${CMAKE_SOURCE_DIR}/depend/libtheora/win32/VS2010/Win32/Debug/libtheora_static.lib"
            "${CMAKE_SOURCE_DIR}/depend/libtheora/win32/VS2010/Win32/Debug/libtheorafile_static.lib")
        SET(THEORA_LIBRARIES_RELEASE
            "${CMAKE_SOURCE_DIR}/depend/libtheora/win32/VS2010/Win32/Release/libtheora_static.lib"
            "${CMAKE_SOURCE_DIR}/depend/libtheora/win32/VS2010/Win32/Release/libtheorafile_static.lib")
        SET(THEORA_PLATFORM "Win32")
    endif()
    SET(THEORA_LIBRARIES_DEBUG "${THEORA_LIBRARIES_DEBUG}")
    SET(THEORA_LIBRARIES_RELEASE "${THEORA_LIBRARIES_RELEASE}")

    FILE(REMOVE_RECURSE
        ${CMAKE_SOURCE_DIR}/depend/libtheora/win32/VS2010/UpgradeLog.htm
        ${CMAKE_SOURCE_DIR}/depend/libtheora/win32/VS2010/Backup
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${THEORA_LIBRARIES_DEBUG}
        COMMAND ${CMAKE_VS_DEVENV_COMMAND} /Upgrade win32/VS2010/theora_static.sln
        COMMAND ${CMAKE_VS_MSBUILD_COMMAND} win32/VS2010/theora_static.sln /t:Build /p:WarningLevel=0 /p:Configuration=Debug /p:Platform=${THEORA_PLATFORM}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/depend/libtheora
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${THEORA_LIBRARIES_RELEASE}
        COMMAND ${CMAKE_VS_DEVENV_COMMAND} /Upgrade win32/VS2010/theora_static.sln;
        COMMAND ${CMAKE_VS_MSBUILD_COMMAND} win32/VS2010/theora_static.sln /t:Build /p:WarningLevel=0 /p:Configuration=Release /p:Platform=${THEORA_PLATFORM}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/depend/libtheora
    )
    ADD_CUSTOM_TARGET(theora DEPENDS ${THEORA_LIBRARIES_DEBUG} ${THEORA_LIBRARIES_RELEASE})
ELSE()
    MESSAGE(STATUS "Using autogen.sh")
    SET(THEORA_LIBRARIES "${CMAKE_SOURCE_DIR}/depend/libtheora/lib/.libs/libtheora.${SHARED_EXT}")
    SET(THEORA_LIBRARIES "${THEORA_LIBRARIES}")

    SET(THEORA_AUTOCONF_STAMP "${CMAKE_SOURCE_DIR}/depend/libtheora/stamp-h1")
    ADD_CUSTOM_COMMAND(
        OUTPUT ${THEORA_AUTOCONF_STAMP}
        COMMAND ${CMAKE_COMMAND} -E env
                CPPFLAGS="-I${CMAKE_SOURCE_DIR}/depend/libogg/include"
                LDFLAGS="-L${CMAKE_SOURCE_DIR}/depend/libogg/src/.libs"
                ./autogen.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/depend/libtheora
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${THEORA_LIBRARIES}
        MAIN_DEPENDENCY ${THEORA_AUTOCONF_STAMP}
        COMMAND make
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/depend/libtheora
    )
    ADD_CUSTOM_TARGET(theora DEPENDS ogg ${THEORA_LIBRARIES})
ENDIF()
ADD_DEPENDENCIES(depend theora)

]]

### FlatBuffers
MESSAGE("Adding dependency: flatbuffers")

SET(FLATBUFFERS_OPTIONS
    -DFLATBUFFERS_BUILD_TESTS=OFF
    -DFLATBUFFERS_BUILD_FLATHASH=OFF
    -DFLATBUFFERS_INSTALL=OFF
)

SET(FLATBUFFERS_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/depend/flatbuffers/include"
)

FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/depend/flatbuffers)
EXECUTE_PROCESS(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}"
    	${CMAKE_SOURCE_DIR}/depend/flatbuffers ${FLATBUFFERS_OPTIONS}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/flatbuffers
)

IF (MSVC)
    SET(FLATBUFFERS_LIBRARIES_DEBUG "${CMAKE_BINARY_DIR}/depend/flatbuffers/Debug/flatbuffers.lib")
    SET(FLATBUFFERS_LIBRARIES_DEBUG "${FLATBUFFERS_LIBRARIES_DEBUG}")
    SET(FLATBUFFERS_LIBRARIES_RELEASE "${CMAKE_BINARY_DIR}/depend/flatbuffers/Release/flatbuffers.lib")
    SET(FLATBUFFERS_LIBRARIES_RELEASE "${FLATBUFFERS_LIBRARIES_RELEASE}")

    ADD_CUSTOM_COMMAND(
        OUTPUT ${FLATBUFFERS_LIBRARIES_DEBUG}
        COMMAND ${CMAKE_COMMAND} --build . --config Debug --target flatbuffers
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/flatbuffers
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${FLATBUFFERS_LIBRARIES_RELEASE}
        COMMAND ${CMAKE_COMMAND} --build . --config Release --target flatbuffers
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/flatbuffers
    )
    ADD_CUSTOM_TARGET(flatbuffers
        DEPENDS ${FLATBUFFERS_LIBRARIES_DEBUG} ${FLATBUFFERS_LIBRARIES_RELEASE})
ELSE()
    SET(FLATBUFFERS_LIBRARIES "${CMAKE_BINARY_DIR}/depend/flatbuffers/libflatbuffers.a")
    SET(FLATBUFFERS_LIBRARIES "${FLATBUFFERS_LIBRARIES}")

    ADD_CUSTOM_COMMAND(
        OUTPUT ${FLATBUFFERS_LIBRARIES}
        COMMAND ${CMAKE_COMMAND} --build . --target flatbuffers
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/depend/flatbuffers
    )
    ADD_CUSTOM_TARGET(flatbuffers DEPENDS ${FLATBUFFERS_LIBRARIES})
ENDIF()
ADD_DEPENDENCIES(depend flatbuffers)

SET_TARGET_PROPERTIES(
    depend glm json gsl lua zlib sdl2 openal flatbuffers
    PROPERTIES FOLDER "depend"
)

###
### Exported Variables
###

SET(_DEPEND_INCLUDE_DIRS
    ${OPENGL_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${FLATBUFFERS_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${GSL_INCLUDE_DIRS}
    ${OPENAL_INCLUDE_DIRS}
    ${OGG_INCLUDE_DIRS}
    ${VORBIS_INCLUDE_DIRS}
    ${THEORA_INCLUDE_DIRS}
    PARENT_SCOPE
)

SET(_DEPEND_LIBRARIES
    ${OPENGL_LIBRARIES}
    ${LUA_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${SDL2_LIBRARIES}
    ${FLATBUFFERS_LIBRARIES}
    ${OPENAL_LIBRARIES}
    ${OGG_LIBRARIES}
    ${VORBIS_LIBRARIES}
    ${THEORA_LIBRARIES}
    PARENT_SCOPE
)

SET(_DEPEND_LIBRARIES_DEBUG
    ${LUA_LIBRARIES_DEBUG}
    ${ZLIB_LIBRARIES_DEBUG}
    ${SDL2_LIBRARIES_DEBUG}
    ${FLATBUFFERS_LIBRARIES_DEBUG}
    ${OPENAL_LIBRARIES_DEBUG}
    ${OGG_LIBRARIES_DEBUG}
    ${VORBIS_LIBRARIES_DEBUG}
    ${THEORA_LIBRARIES_DEBUG}
    PARENT_SCOPE
)

SET(_DEPEND_LIBRARIES_RELEASE
    ${LUA_LIBRARIES_RELEASE}
    ${ZLIB_LIBRARIES_RELEASE}
    ${SDL2_LIBRARIES_RELEASE}
    ${FLATBUFFERS_LIBRARIES_RELEASE}
    ${OPENAL_LIBRARIES_RELEASE}
    ${OGG_LIBRARIES_RELEASE}
    ${VORBIS_LIBRARIES_RELEASE}
    ${THEORA_LIBRARIES_RELEASE}
    PARENT_SCOPE
)

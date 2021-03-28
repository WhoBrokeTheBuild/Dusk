CMAKE_MINIMUM_REQUIRED(VERSION 3.15 FATAL_ERROR)

# Allow for custom CMake modules
LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/CMake")

INCLUDE(Policies)

###
### Project
###

PROJECT(Dusk)
INCLUDE(Version)

###
### Options
###

OPTION(
    BUILD_TESTS
    "Build Tests"
    OFF
)

OPTION(
    BUILD_DEMOS
    "Build Demo Projects. Enabling this will set BUILD_MODULES=All."
    ON
)

OPTION(
    BUILD_THIRDPARTY
    "Build Missing ThirdParty Dependencies"
    ON
)

OPTION(
    BUILD_PACKAGES
    "Build Packages"
    OFF
)

###
### General Configuration
###

IF(BUILD_TESTS)
    ENABLE_TESTING()
ENDIF()

# Allow for custom organization of files in VisualStudio, Code::Blocks, etc.
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Don't use the install RPATH when building
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

IF(MSVC)
    # Disable Default Exception Handling
    STRING(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
ENDIF()

###
### Modules and Dependencies
###

SET(
    BUILD_MODULES 
    "All" 
    CACHE STRING
    "Semicolon-separated list of required modules, use 'All' to build all modules"
)

# Generate list of available modules
SET(DUSK_AVAILABLE_MODULES
    "FreeType"
    "GLTF2"
    "OpenAL"
    "OpenGL"
    "SDL2"
    "STBI"
    "TinyOBJ"
    "Vulkan"
)
IF(WIN32)
    LIST(APPEND DUSK_AVAILABLE_MODULES "DirectX")
ENDIF()

IF(BUILD_DEMOS OR BUILD_MODULES STREQUAL "All")
    SET(BUILD_MODULES "${DUSK_AVAILABLE_MODULES}")
ENDIF()

# Filter requested modules that are not available
SET(DUSK_REQUIRED_MODULES "")
FOREACH(_module ${BUILD_MODULES})
    IF(${_module} IN_LIST DUSK_AVAILABLE_MODULES)
        LIST(APPEND DUSK_REQUIRED_MODULES ${_module})
    ELSE()
        MESSAGE(WARNING "Module '${_module}' not supported on this platform")
    ENDIF()
ENDFOREACH()

# Generate list of required third party dependencies
SET(DUSK_REQUIRED_DEPENDENCIES
    "glm"
    "GSL"
    "nlohmann_json"
    "cflags"
    "fmt"
    "Python3"
)

IF("OpenGL" IN_LIST DUSK_REQUIRED_MODULES)
    LIST(APPEND DUSK_REQUIRED_MODULES "SDL2")
ENDIF()

IF("Vulkan" IN_LIST DUSK_REQUIRED_MODULES)
    LIST(APPEND DUSK_REQUIRED_DEPENDENCIES "VulkanSDK")
    LIST(APPEND DUSK_REQUIRED_MODULES "SDL2")
ENDIF()

IF("SDL2" IN_LIST DUSK_REQUIRED_MODULES)
    LIST(APPEND DUSK_REQUIRED_DEPENDENCIES "SDL2")
ENDIF()

IF("OpenAL" IN_LIST DUSK_REQUIRED_MODULES)
    LIST(APPEND DUSK_REQUIRED_DEPENDENCIES "OpenAL")
ENDIF()

IF("FreeType" IN_LIST DUSK_REQUIRED_MODULES)
    LIST(APPEND DUSK_REQUIRED_DEPENDENCIES "freetype")
ENDIF()

IF("Archive" IN_LIST DUSK_REQUIRED_MODULES)
ENDIF()

IF("DirectX" IN_LIST DUSK_REQUIRED_MODULES)
    LIST(APPEND DUSK_REQUIRED_DEPENDENCIES "WindowsSDK")
ENDIF()

IF(BUILD_TESTS)
    LIST(APPEND DUSK_REQUIRED_DEPENDENCIES "GTest")
ENDIF()

LIST(REMOVE_DUPLICATES DUSK_REQUIRED_MODULES)
LIST(REMOVE_DUPLICATES DUSK_REQUIRED_DEPENDENCIES)

MESSAGE(STATUS "Available modules: ${DUSK_AVAILABLE_MODULES}")
MESSAGE(STATUS "Building Modules: ${DUSK_REQUIRED_MODULES}")
MESSAGE(STATUS "Required Dependencies: ${DUSK_REQUIRED_DEPENDENCIES}")

###
### Third Party Dependencies
###

IF(BUILD_THIRDPARTY)
    SET(_thirdparty_source_dir ${CMAKE_SOURCE_DIR}/ThirdParty)
    SET(_thirdparty_binary_dir ${CMAKE_BINARY_DIR}/ThirdParty)

    FILE(MAKE_DIRECTORY ${_thirdparty_binary_dir})

    # Set _ROOT variables for config-based packages
    SET(glm_ROOT            ${_thirdparty_binary_dir}/glm/cmake)
    SET(nlohmann_json_ROOT  ${_thirdparty_binary_dir}/nlohmann_json)
    SET(cflags_ROOT         ${_thirdparty_binary_dir}/cflags)
    SET(SDL2_ROOT           ${_thirdparty_binary_dir}/SDL2)
    SET(OpenAL_ROOT         ${_thirdparty_binary_dir}/OpenAL)
    SET(freetype_ROOT       ${_thirdparty_binary_dir}/freetype)
    SET(fmt_ROOT            ${_thirdparty_binary_dir}/fmt)

    # Set CMAKE_PREFIX_PATH for find-based packages
    LIST(APPEND CMAKE_PREFIX_PATH
        ${_thirdparty_binary_dir}/GSL
        ${_thirdparty_binary_dir}/Python3
        ${_thirdparty_binary_dir}/libogg
        ${_thirdparty_binary_dir}/libvorbis
        ${_thirdparty_binary_dir}/libtheora
        ${_thirdparty_binary_dir}/GTest
    )

    # CMake refuses to quote the list when passing it on the command line, and `;` will
    # split the command in half, so we have to switch them to a different separator
    STRING(REPLACE ";" ":" _tmp_required_dependencies "${DUSK_REQUIRED_DEPENDENCIES}")

    EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" 
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DDUSK_REQUIRED_DEPENDENCIES=${_tmp_required_dependencies}
            ${_thirdparty_source_dir}
        WORKING_DIRECTORY ${_thirdparty_binary_dir}
        RESULT_VARIABLE _result
    )
    IF(_result)
        MESSAGE("${_result}")
        MESSAGE(FATAL_ERROR "Failed to configure ThirdParty projects")
    ENDIF()

    SET(_thirdparty_build_command ${CMAKE_COMMAND} --build . )

    # If we're using `make`, we can build with extra cores
    IF(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
        SET(_thirdparty_build_command ${CMAKE_MAKE_PROGRAM} -l )
    ENDIF()

    EXECUTE_PROCESS(
        COMMAND ${_thirdparty_build_command}
        WORKING_DIRECTORY ${_thirdparty_binary_dir}
        RESULT_VARIABLE _result
    )

    IF(_result)
        MESSAGE("${_result}")
        MESSAGE(FATAL_ERROR "Failed to build ThirdParty projects")
    ENDIF()
ENDIF()

IF("glm" IN_LIST DUSK_REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(glm CONFIG REQUIRED)

    IF(NOT TARGET glm::glm)
        MESSAGE(SEND_ERROR "Incompatible version of glm found")
    ENDIF()
ENDIF()

IF("GSL" IN_LIST DUSK_REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(GSL REQUIRED)
ENDIF()

IF("nlohmann_json" IN_LIST DUSK_REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(nlohmann_json CONFIG REQUIRED)
ENDIF()

IF("cflags" IN_LIST DUSK_REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(cflags CONFIG REQUIRED)
ENDIF()

IF("fmt" IN_LIST DUSK_REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(fmt CONFIG REQUIRED)
ENDIF()

IF("Python3" IN_LIST DUSK_REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(Python3 3.8 REQUIRED
        COMPONENTS Interpreter Development)
ENDIF()

IF("SDL2" IN_LIST DUSK_REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(SDL2 2.0.6 CONFIG REQUIRED)
ENDIF()

IF("VulkanSDK" IN_LIST DUSK_REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(VulkanSDK REQUIRED)
ENDIF()

IF("WindowsSDK" IN_LIST DUSK_REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(WindowsSDK REQUIRED)
ENDIF()

IF("OpenAL" IN_LIST DUSK_REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(OpenAL CONFIG REQUIRED)
ENDIF()

IF("freetype" IN_LIST DUSK_REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(freetype CONFIG REQUIRED)
ENDIF()

IF("Ogg" IN_LIST DUSK_REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(Ogg REQUIRED 
        COMPONENTS Vorbis Theora)
ENDIF()

IF("GTest" IN_LIST DUSK_REQUIRED_DEPENDENCIES)
    SET(GTEST_MSVC_SEARCH "MD")
    FIND_PACKAGE(GTest REQUIRED)
ENDIF()

FIND_PROGRAM(gdb_COMMAND gdb)
FIND_PROGRAM(valgrind_COMMAND valgrind)

# Compile list of runtime directories

IF(TARGET SDL2::SDL2)
    GET_TARGET_PROPERTY(_location SDL2::SDL2 IMPORTED_LOCATION)
    GET_FILENAME_COMPONENT(SDL2_RUNTIME_DIR ${_location} DIRECTORY)
    
    IF (SDL2_RUNTIME_DIR STREQUAL "")
        STRING(TOUPPER ${CMAKE_BUILD_TYPE} _type)
        GET_TARGET_PROPERTY(_location SDL2::SDL2 IMPORTED_LOCATION_${_type})
        GET_FILENAME_COMPONENT(SDL2_RUNTIME_DIR ${_location} DIRECTORY)
    ENDIF()

    MESSAGE(STATUS ${SDL2_RUNTIME_DIR} "IMPORTED_LOCATION_${CMAKE_BUILD_TYPE}")
ENDIF()

IF(TARGET OpenAL::OpenAL)
    GET_TARGET_PROPERTY(_location OpenAL::OpenAL IMPORTED_LOCATION)
    GET_FILENAME_COMPONENT(OpenAL_RUNTIME_DIR ${_location} DIRECTORY)
    
    IF (OpenAL_RUNTIME_DIR STREQUAL "")
        STRING(TOUPPER ${CMAKE_BUILD_TYPE} _type)
        GET_TARGET_PROPERTY(_location OpenAL::OpenAL IMPORTED_LOCATION_${_type})
        GET_FILENAME_COMPONENT(OpenAL_RUNTIME_DIR ${_location} DIRECTORY)
    ENDIF()
ENDIF()

###
### Globals
###

# Allow custom targets in Visual Studio to find .dll files
SET(DUSK_MODULE_PATH
    ${SDL2_RUNTIME_DIR}
    ${OpenAL_RUNTIME_DIR}
    ${Python3_RUNTIME_LIBRARY_DIRS}
    ${GTEST_RUNTIME_DIR}
)

SET(DUSK_MODULE_TARGETS "")
SET(DUSK_ENGINE_SHADER_INCLUDES "")

###
### Engine
###

ADD_SUBDIRECTORY(Engine)

###
### Modules
###

ADD_SUBDIRECTORY(Modules)

###
### Demos
###

IF(BUILD_DEMOS)
    ADD_SUBDIRECTORY(Demos)
ENDIF()

###
### Install Configuration
###

INSTALL(
    TARGETS ${DUSK_MODULE_TARGETS}
    EXPORT DuskTargets
    INCLUDES DESTINATION include
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

INCLUDE(CMakePackageConfigHelpers)

WRITE_BASIC_PACKAGE_VERSION_FILE(
    "${CMAKE_BINARY_DIR}/DuskConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

INSTALL(EXPORT DuskTargets
    FILE DuskTargets.cmake
    NAMESPACE Dusk::
    DESTINATION lib/cmake/Dusk
)

CONFIGURE_PACKAGE_CONFIG_FILE(
    "${CMAKE_SOURCE_DIR}/CMake/DuskConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/DuskConfig.cmake"
    INSTALL_DESTINATION lib/cmake/Dusk
)

INSTALL(EXPORT DuskTargets 
    DESTINATION lib/cmake/Dusk
)

INSTALL(FILES 
    "${CMAKE_BINARY_DIR}/DuskConfigVersion.cmake" 
    "${CMAKE_BINARY_DIR}/DuskConfig.cmake"
    DESTINATION lib/cmake/Dusk
)

FIND_PACKAGE(PkgConfig QUIET)

IF(PKG_CONFIG_FOUND)
    CONFIGURE_FILE(
        "${CMAKE_SOURCE_DIR}/CMake/Dusk.pc.in"
        "${CMAKE_BINARY_DIR}/Dusk.pc"
        @ONLY
    )

    INSTALL(FILES
        "${CMAKE_BINARY_DIR}/Dusk.pc"
        DESTINATION share/pkgconfig
    )
ENDIF()

###
### Packaging
###

IF(BUILD_PACKAGES)
    ADD_SUBDIRECTORY(Packages)
ENDIF()

###
### Docker
###

FIND_PROGRAM(docker_COMMAND docker)

IF(docker_COMMAND)
    ADD_CUSTOM_TARGET(
        build-docker-images
        COMMAND ${docker_COMMAND} build --target builder-minimal -t whobrokethebuild/dusk:builder-minimal .
        COMMAND ${docker_COMMAND} build --target builder -t whobrokethebuild/dusk:builder .
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    SET_TARGET_PROPERTIES(
        build-docker-images
        PROPERTIES 
            FOLDER "Automation"
    )

    ADD_CUSTOM_TARGET(
        push-docker-images
        COMMAND ${docker_COMMAND} push whobrokethebuild/dusk:builder-minimal
        COMMAND ${docker_COMMAND} push whobrokethebuild/dusk:builder
        DEPENDS build-docker-images
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    SET_TARGET_PROPERTIES(
        push-docker-images
        PROPERTIES 
            FOLDER "Automation"
    )
ENDIF()

###
### CI
###

FIND_PROGRAM(circleci_COMMAND circleci)

IF(circleci_COMMAND)
    ADD_CUSTOM_TARGET(
        test-circleci
        COMMAND ${circleci_COMMAND} local execute --job "Clang, Release, System"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    SET_TARGET_PROPERTIES(
        test-circleci
        PROPERTIES 
            FOLDER "Automation"
    )
ENDIF()
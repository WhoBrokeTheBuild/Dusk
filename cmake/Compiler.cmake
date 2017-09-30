
###
### Compiler-specific flags
###

# GCC or Clang
IF(CMAKE_COMPILER_IS_GNUCC OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")

    INCLUDE(CheckCXXCompilerFlag)

    CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
    CHECK_CXX_COMPILER_FLAG("-std=c++1y" COMPILER_SUPPORTS_CXX1Y)

    IF(COMPILER_SUPPORTS_CXX14)
        SET(CMAKE_CXX_FLAGS "-std=c++14")
    ELSEIF(COMPILER_SUPPORTS_CXX1Y)
        SET(CMAKE_CXX_FLAGS "-std=c++1y")
    ELSE()
        MESSAGE(FATAL_ERROR "Compiler '${CMAKE_CXX_COMPILER}' has no C++14 support")
    ENDIF()

    ADD_DEFINITIONS(-D_XOPEN_SOURCE=700)
    ADD_COMPILE_OPTIONS(-Wall -Wextra -pedantic -Wno-unused-parameter
        -Wno-missing-braces -Wno-unused-function -Wno-variadic-macros)

# VisualStudio
ELSEIF(MSVC)

    ADD_DEFINITIONS(/D_CRT_SECURE_NO_WARNINGS)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
    SET(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")

ENDIF()

# Clang
IF(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    ADD_COMPILE_OPTIONS(-Wno-gnu-zero-variadic-macro-arguments
        -Wno-gnu-statement-expression -Wno-keyword-macro)
ENDIF()

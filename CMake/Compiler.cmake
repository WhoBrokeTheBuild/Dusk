
###
### Compiler-specIFic flags
###

# GCC or Clang

    #ADD_DEFINITIONS(-D_XOPEN_SOURCE=700)
    #ADD_COMPILE_OPTIONS(cxx_std_17 -Wall -Wextra -Wno-unused-parameter
    #    -Wno-unused-variable -Wno-missing-braces -Wno-unused-function
    #    -Wno-variadic-macros)

# VisualStudio
IF(MSVC)

    ADD_DEFINITIONS(/D_CRT_SECURE_NO_WARNINGS)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
    SET(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")

    IF(MSVC70 OR MSVC71)
      set(MSVC_PREFIX "vc70")
    ELSEIF(MSVC80)
      set(MSVC_PREFIX "vc80")
    ELSEIF(MSVC90)
      set(MSVC_PREFIX "vc90")
    ELSEIF(MSVC10)
      set(MSVC_PREFIX "vc100")
    ELSEIF(MSVC11)
      set(MSVC_PREFIX "vc110")
    ELSEIF(MSVC12)
      set(MSVC_PREFIX "vc120")
    ELSEIF(MSVC14)
      set(MSVC_PREFIX "vc140")
    ELSE()
      set(MSVC_PREFIX "vc150")
    ENDIF()

ENDIF()

# Clang
IF(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    ADD_COMPILE_OPTIONS(-Wno-gnu-zero-variadic-macro-arguments
        -Wno-gnu-statement-expression -Wno-keyword-macro)
ENDIF()

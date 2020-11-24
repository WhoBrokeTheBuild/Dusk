
SET(_target HelloWorld)

DEFINE_DEMO(${_target})

ADD_DEPENDENCIES(
    ${_target}
    DuskOpenGL
    DuskSTBI
    DuskTinyOBJ
)

IF(TARGET DuskDirectX)
    ADD_DEPENDENCIES(
        ${_target}
        DuskDirectX
    )
ENDIF()
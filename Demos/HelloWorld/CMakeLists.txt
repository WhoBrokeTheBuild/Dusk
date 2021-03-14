
SET(_target HelloWorld)

DEFINE_DEMO(${_target})

ADD_DEPENDENCIES(
    ${_target}
    DuskOpenGL
    DuskSTBI
    DuskTinyOBJ
    DuskGLTF2
)

IF(TARGET DuskVulkan)
    ADD_DEPENDENCIES(
        ${_target}
        DuskVulkan
    )
ENDIF()

IF(TARGET DuskDirectX)
    ADD_DEPENDENCIES(
        ${_target}
        DuskDirectX
    )
ENDIF()
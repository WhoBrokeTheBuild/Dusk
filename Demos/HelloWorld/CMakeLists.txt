
SET(_TARGET HelloWorld)

DUSK_DEMO(${_TARGET})

TARGET_LINK_LIBRARIES(
    ${_TARGET}
    DuskOpenGL
)

ADD_DEPENDENCIES(
    ${_TARGET} 
    DuskSTBI 
    DuskOpenGL
)

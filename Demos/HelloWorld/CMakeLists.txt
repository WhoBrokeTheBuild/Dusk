
SET(_TARGET HelloWorld)

Demo(${_TARGET})

TARGET_LINK_LIBRARIES(
    ${_TARGET}
    DuskOpenGL
)

ADD_DEPENDENCIES(
    ${_TARGET} 
    DuskSTBI 
    DuskOpenGL
)

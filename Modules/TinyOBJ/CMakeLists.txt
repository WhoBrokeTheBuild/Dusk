
SET(_TARGET DuskTinyOBJ)

Module(${_TARGET} TINYOBJ)

TARGET_LINK_LIBRARIES(
    ${_TARGET}
    PUBLIC
        DuskCore
)

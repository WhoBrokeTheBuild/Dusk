
SET(_TARGET DuskTinyOBJ)

Module(${_TARGET} TinyOBJ)

TARGET_LINK_LIBRARIES(
    ${_TARGET}
    PUBLIC
        DuskCore
)

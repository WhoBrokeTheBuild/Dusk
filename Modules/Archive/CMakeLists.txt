
SET(_TARGET DuskArchive)

Module(${_TARGET} ARCHIVE)

TARGET_LINK_LIBRARIES(
    ${_TARGET}
    PUBLIC
        DuskCore
)
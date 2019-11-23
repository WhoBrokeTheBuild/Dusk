
SET(_TARGET DuskCore)

Module(${_TARGET})

TARGET_LINK_LIBRARIES(
    ${_TARGET}
    PUBLIC
        GLM::GLM
        JSON::JSON
        Python3::Python
        Threads::Threads
)
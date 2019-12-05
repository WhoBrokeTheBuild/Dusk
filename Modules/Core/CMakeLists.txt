
SET(_TARGET DuskCore)

Module(${_TARGET} CORE)

TARGET_LINK_LIBRARIES(
    ${_TARGET}
    PUBLIC
        glm::glm
        nlohmann_json::nlohmann_json
        Python3::Python
        Threads::Threads
)

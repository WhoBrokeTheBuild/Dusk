
INCLUDE(DefineModule)
INCLUDE(CompileShaders)
INCLUDE(SetSourceGroups)

SET(_target DuskCore)

DEFINE_MODULE(${_target} CORE)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        glm::glm
        GSL::GSL
        nlohmann_json::nlohmann_json
        cflags::cflags
        fmt::fmt
        Python3::Python
        ${CMAKE_DL_LIBS}
)
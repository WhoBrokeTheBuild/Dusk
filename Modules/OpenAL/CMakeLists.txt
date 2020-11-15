
SET(_target DuskOpenAL)

DEFINE_MODULE(${_target} OPENAL)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        OpenAL::OpenAL
)
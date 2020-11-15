
SET(_target DuskFreeType)

DEFINE_MODULE(${_target} FREETYPE)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        freetype
)
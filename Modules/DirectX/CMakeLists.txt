
SET(_target DuskDirectX)

DEFINE_MODULE(${_target} DIRECTX)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        d3d12.lib
        dxgi.lib
        d3dcompiler.lib
)

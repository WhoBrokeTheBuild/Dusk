#ifndef DUSK_DIRECTX_HPP
#define DUSK_DIRECTX_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_DIRECTX_EXPORT)
    #define DUSK_DIRECTX_API DUSK_API_EXPORT
#else
    #define DUSK_DIRECTX_API DUSK_API_IMPORT
#endif

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
    #define NOMINMAX
#endif

#include <Windows.h>

// Windows Runtime Template Library
// Required for ComPtr
#include <wrl.h>

using Microsoft::WRL::ComPtr;

// DirectX 12
#include <d3d12.h>
#include <dxcapi.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>

#endif // DUSK_DIRECTX_HPP

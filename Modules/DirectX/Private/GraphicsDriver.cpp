#include <Dusk/DirectX/GraphicsDriver.hpp>
#include <Dusk/Dusk.hpp>
#include <Dusk/Log.hpp>

// DirectX 12 Extension Library
#include "d3dx12.h"

namespace Dusk {

LRESULT CALLBACK WndProc(HWND hwnd, unsigned msg, WPARAM wParam, LPARAM lParam)
{
    DirectXGraphicsDriver * gfx = DUSK_DIRECTX_GRAPHICS_DRIVER(GetGraphicsDriver());
    return gfx->ProcessMessage(hwnd, msg, wParam, lParam);
}

DUSK_DIRECTX_API
DirectXGraphicsDriver::DirectXGraphicsDriver()
{
    HRESULT result;

    HINSTANCE inst = GetModuleHandle(nullptr);

    std::string name = GetApplicationName();

    // Create Window

    WNDCLASSEX wndClass = { };
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.hInstance = inst;
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = &WndProc;
    wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wndClass.hIconSm = nullptr;
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = name.c_str();
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;

    if (!RegisterClassEx(&wndClass)) {
        DuskLogError("RegisterClassEx failed: %d", GetLastError());
        return;
    }

    std::string title = name + " (" + GetApplicationVersion().GetString() + ")";

    _window = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        name.c_str(),
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        640, 480,
        nullptr, nullptr,
        (HINSTANCE)GetModuleHandle(nullptr),
        nullptr
    );

    ShowWindow(_window, SW_SHOWNORMAL);
    UpdateWindow(_window);

    // Create Pipeline

    ComPtr<IDXGIFactory6> factory;
    result = CreateDXGIFactory2(0, IID_PPV_ARGS(&factory));
    if (FAILED(result)) {
        DuskLogError("CreateDXGIFactory2 failed: %d", GetLastError());
        return;
    }

    ComPtr<IDXGIAdapter1> adapter = nullptr;

    for (unsigned i = 0; ; ++i) {
        result = factory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));
        if (result == DXGI_ERROR_NOT_FOUND) {
            break;
        }

        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            // Skip the Basic Render Driver adapter
            continue;
        }

        result = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, __uuidof(ID3D12Device), nullptr);
        if (SUCCEEDED(result)) {
            break;
        }

        adapter->Release();
    }

    if (!adapter) {
        DuskLogError("Failed to find a viable DXGI Adapter");
        return;
    }

    ComPtr<IDXGIAdapter1> hardwareAdapter = adapter.Detach();

    result = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&_device));
    if (FAILED(result)) {
        DuskLogError("Failed to create D3D12 Device");
        return;
    }

    D3D12_COMMAND_QUEUE_DESC queueDesc = { };
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    _device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_commandQueue));

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { };
    swapChainDesc.BufferCount = BUFFER_COUNT;
    swapChainDesc.Width = 640;
    swapChainDesc.Height = 480;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain;
    result = factory->CreateSwapChainForHwnd(
        _commandQueue.Get(),
        _window,
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain
    );
    if (FAILED(result)) {
        DuskLogError("Failed to create swap chain");
        return;
    }

    // Disable Alt+Enter fullscreen command
    result = factory->MakeWindowAssociation(_window, DXGI_MWA_NO_ALT_ENTER);
    if (FAILED(result)) {
        DuskLogWarn("Failed to disable Alt+Enter");
    }
    
    // Convert the IDXGISwapChain1 into IDXGISwapChain4
    swapChain.As(&_swapChain);

    _renderTargetIndex = _swapChain->GetCurrentBackBufferIndex();

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = { };
    rtvHeapDesc.NumDescriptors = BUFFER_COUNT;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    result = _device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_rtvHeap));
    if (FAILED(result)) {
        DuskLogError("Failed to create RTV descriptor heap");
        return;
    }

    _rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());

    for (unsigned i = 0; i < BUFFER_COUNT; ++i) {
        result = _swapChain->GetBuffer(i, IID_PPV_ARGS(&_renderTargets[i]));
        if (FAILED(result)) {
            DuskLogError("Failed to get buffer %d from swap chain", i);
            return;
        }

        _device->CreateRenderTargetView(_renderTargets[i].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, _rtvDescriptorSize);
    }

    result = _device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_commandAllocator));
    if (FAILED(result)) {
        DuskLogError("Failed to create command allocator");
        return;
    }

    



    _device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocator.Get(), nullptr, IID_PPV_ARGS(&_commandList));
    _commandList->Close();

    _device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
    _fenceValue = 1;

    _fenceEvent = CreateEvent(nullptr, false, false, nullptr);

}

DUSK_DIRECTX_API
DirectXGraphicsDriver::~DirectXGraphicsDriver()
{
    if (_window) {
        DestroyWindow(_window);
        _window = nullptr;
    }
}

void DirectXGraphicsDriver::SetWindowTitle(const std::string& title)
{
    SetWindowText(_window, title.c_str());
}

std::string DirectXGraphicsDriver::GetWindowTitle()
{
    char buffer[256];
    GetWindowText(_window, buffer, sizeof(buffer));
    return std::string(buffer);
}

void DirectXGraphicsDriver::SetWindowSize(const ivec2& size)
{
    SetWindowPos(_window, HWND_TOP, 0, 0, size.x, size.y, SWP_NOMOVE | SWP_NOREPOSITION);
}

ivec2 DirectXGraphicsDriver::GetWindowSize()
{
    RECT rect;
    GetWindowRect(_window, &rect);
    return { rect.right - rect.left, rect.bottom - rect.top };
}

void DirectXGraphicsDriver::ProcessEvents()
{
    MSG Msg;
    if (GetMessage(&Msg, NULL, 0, 0)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
}

void DirectXGraphicsDriver::SwapBuffers()
{
    _commandAllocator->Reset();
    _commandList->Reset(_commandAllocator.Get(), _pipelineState.Get());

    // _commandList->ResourceBarrier(1, 
    //     &CD3DX12_RESOURCE_BARRIER::Transition(_renderTargets[_frameIndex].Get(), 
    //         D3D12_RESOURCE_STATE_PRESENT,
    //         D3D12_RESOURCE_STATE_RENDER_TARGET));

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart(), _renderTargetIndex, _rtvDescriptorSize);

    const float clearColor[] = { 0.392f, 0.584f, 0.929f, 1.0f };
    _commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    // _commandList->ResourceBarrier(1, 
    //     &CD3DX12_RESOURCE_BARRIER::Transition(_renderTargets[_frameIndex].Get(),
    //         D3D12_RESOURCE_STATE_RENDER_TARGET,
    //         D3D12_RESOURCE_STATE_PRESENT));

    _commandList->Close();

    ID3D12CommandList * tmp[] = { _commandList.Get() };
    _commandQueue->ExecuteCommandLists(_countof(tmp), tmp);

    _swapChain->Present(1, 0);


    // TODO Remove
    unsigned long fence = _fenceValue;
    _commandQueue->Signal(_fence.Get(), fence);
    ++_fenceValue;

    if (_fence->GetCompletedValue() < fence) {
        _fence->SetEventOnCompletion(fence, _fenceEvent);
        WaitForSingleObject(_fenceEvent, INFINITE);
    }

    _renderTargetIndex = _swapChain->GetCurrentBackBufferIndex();
}

std::shared_ptr<Texture> DirectXGraphicsDriver::CreateTexture()
{
    return nullptr;
}

std::shared_ptr<Shader> DirectXGraphicsDriver::CreateShader()
{
    return std::shared_ptr<Shader>(New DirectXShader());
}

std::shared_ptr<Mesh> DirectXGraphicsDriver::CreateMesh()
{
    return std::shared_ptr<Mesh>(New DirectXMesh());
}

bool DirectXGraphicsDriver::SetShaderData(const std::string& name, size_t size, void * data)
{
//     ID3D11ShaderReflection * reflector = nullptr;
//     D3DReflect()


//     const auto& it = _constantBufferBindings.find(binding);
//     if (it == _constantBufferBindings.cend()) {
//         GetConstantBufferByName()
//     }

    return false;
}

LRESULT DirectXGraphicsDriver::ProcessMessage(HWND hwnd, unsigned msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CLOSE:
        SetRunning(false);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}


} // namespace Dusk
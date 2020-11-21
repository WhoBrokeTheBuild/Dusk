#include <Dusk/DirectX/GraphicsDriver.hpp>
#include <Dusk/Dusk.hpp>
#include <Dusk/Log.hpp>

#include "d3dx12.h"
#include <d3d11shader.h>

namespace Dusk::DirectX {

LRESULT CALLBACK WndProc(HWND hwnd, unsigned msg, WPARAM wParam, LPARAM lParam)
{
    GraphicsDriver* gfx = (GraphicsDriver*)GetGraphicsDriver();
    return gfx->ProcessMessage(hwnd, msg, wParam, lParam);
}

DUSK_DIRECTX_API
GraphicsDriver::GraphicsDriver()
{
    std::string name = GetApplicationName();

    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = (HINSTANCE)GetModuleHandle(nullptr);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = name.c_str();
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;

    if (!RegisterClassEx(&wc)) {
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

    unsigned factoryFlags = 0;
    ComPtr<IDXGIFactory4> factory;
    CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory));

    ComPtr<IDXGIAdapter1> adapter = nullptr;

    for (unsigned adapterIndex = 0; 
        DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(adapterIndex, &adapter); 
        ++adapterIndex) {

        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            // Skip software render adapter
            continue;
        }

        HRESULT result = D3D12CreateDevice(
            adapter.Get(),
            D3D_FEATURE_LEVEL_11_0,
            _uuidof(ID3D12Device), 
            nullptr
        );

        if (SUCCEEDED(result)) {
            break;
        }
    }

    ComPtr<IDXGIAdapter1> hardwareAdapter = adapter.Detach();
        
    D3D12CreateDevice(
        hardwareAdapter.Get(),
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&_device)
    );

    D3D12_COMMAND_QUEUE_DESC queueDesc = { };
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    _device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_commandQueue));

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { };
    swapChainDesc.BufferCount = FRAME_COUNT;
    swapChainDesc.Width = 640;
    swapChainDesc.Height = 480;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain;
    factory->CreateSwapChainForHwnd(
        _commandQueue.Get(),
        _window,
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain
    );

    factory->MakeWindowAssociation(_window, DXGI_MWA_NO_ALT_ENTER);
    
    swapChain.As(&_swapChain);
    _frameIndex = _swapChain->GetCurrentBackBufferIndex();

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = { };
    rtvHeapDesc.NumDescriptors = FRAME_COUNT;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    _device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_rtvHeap));
    _rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());

    for (unsigned n = 0; n < FRAME_COUNT; ++n) {
        _swapChain->GetBuffer(n, IID_PPV_ARGS(&_renderTargets[n]));
        _device->CreateRenderTargetView(_renderTargets[n].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, _rtvDescriptorSize);
    }

    _device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_commandAllocator));


    _device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocator.Get(), nullptr, IID_PPV_ARGS(&_commandList));
    _commandList->Close();

    _device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
    _fenceValue = 1;

    _fenceEvent = CreateEvent(nullptr, false, false, nullptr);

}

DUSK_DIRECTX_API
GraphicsDriver::~GraphicsDriver()
{

    if (_window) {
        DestroyWindow(_window);
        _window = nullptr;
    }
}

void GraphicsDriver::SetWindowTitle(const std::string& title)
{
    SetWindowText(_window, title.c_str());
}

std::string GraphicsDriver::GetWindowTitle()
{
    char buffer[256];
    GetWindowText(_window, buffer, sizeof(buffer));
    return std::string(buffer);
}

void GraphicsDriver::SetWindowSize(const ivec2& size)
{
    SetWindowPos(_window, HWND_TOP, 0, 0, size.x, size.y, SWP_NOMOVE | SWP_NOREPOSITION);
}

ivec2 GraphicsDriver::GetWindowSize()
{
    RECT rect;
    GetWindowRect(_window, &rect);
    return { rect.right - rect.left, rect.bottom - rect.top };
}

void GraphicsDriver::ProcessEvents()
{
    MSG Msg;
    if (GetMessage(&Msg, NULL, 0, 0)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
}

void GraphicsDriver::SwapBuffers()
{
    _commandAllocator->Reset();
    _commandList->Reset(_commandAllocator.Get(), _pipelineState.Get());

    _commandList->ResourceBarrier(1, 
        &CD3DX12_RESOURCE_BARRIER::Transition(_renderTargets[_frameIndex].Get(), 
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET));

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart(), _frameIndex, _rtvDescriptorSize);

    const float clearColor[] = { 0.392f, 0.584f, 0.929f, 1.0f };
    _commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    _commandList->ResourceBarrier(1, 
        &CD3DX12_RESOURCE_BARRIER::Transition(_renderTargets[_frameIndex].Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT));

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

    _frameIndex = _swapChain->GetCurrentBackBufferIndex();
}

std::shared_ptr<Dusk::Texture> GraphicsDriver::CreateTexture()
{
    return nullptr;
}

std::shared_ptr<Dusk::Shader> GraphicsDriver::CreateShader()
{
    return std::make_shared<Shader>();
}

std::shared_ptr<Dusk::Mesh> GraphicsDriver::CreateMesh()
{
    return std::make_shared<Mesh>();
}

bool GraphicsDriver::SetShaderData(const std::string& name, size_t size, void * data)
{
//     ID3D11ShaderReflection * reflector = nullptr;
//     D3DReflect()


//     const auto& it = _constantBufferBindings.find(binding);
//     if (it == _constantBufferBindings.cend()) {
//         GetConstantBufferByName()
//     }

    return false;
}

LRESULT GraphicsDriver::ProcessMessage(HWND hwnd, unsigned msg, WPARAM wParam, LPARAM lParam)
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

} // namespace Dusk::DirectX
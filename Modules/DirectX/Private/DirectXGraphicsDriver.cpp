#include <Dusk/DirectX/DirectXGraphicsDriver.hpp>

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
bool DirectXGraphicsDriver::Initialize()
{
    if (!InitWindow()) {
        return false;
    }

    if (!InitDevice()) {
        return false;
    }

    if (!InitAllocator()) {
        return false;
    }

    // Create Pipeline


    // TODO: Move?
    // D3D12_COMMAND_QUEUE_DESC queueDesc = { };
    // queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    // queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    // _device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_commandQueue));



    return true;
}

DUSK_DIRECTX_API
void DirectXGraphicsDriver::Terminate()
{

    TermWindow();
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
    // _commandAllocator->Reset();
    // _commandList->Reset(_commandAllocator.Get(), _pipelineState.Get());

    // _commandList->ResourceBarrier(1, 
    //     &CD3DX12_RESOURCE_BARRIER::Transition(_renderTargets[_frameIndex].Get(), 
    //         D3D12_RESOURCE_STATE_PRESENT,
    //         D3D12_RESOURCE_STATE_RENDER_TARGET));

    // CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart(), _renderTargetIndex, _rtvDescriptorSize);

    // _commandList->ClearRenderTargetView(rtvHandle, glm::value_ptr(GetClearColor()), 0, nullptr);

    // _commandList->ResourceBarrier(1, 
    //     &CD3DX12_RESOURCE_BARRIER::Transition(_renderTargets[_frameIndex].Get(),
    //         D3D12_RESOURCE_STATE_RENDER_TARGET,
    //         D3D12_RESOURCE_STATE_PRESENT));

    // _commandList->Close();

    // ID3D12CommandList * tmp[] = { _commandList.Get() };
    // _commandQueue->ExecuteCommandLists(_countof(tmp), tmp);

    // _swapChain->Present(1, 0);


    // TODO Remove
    // unsigned long fence = _fenceValue;
    // _commandQueue->Signal(_fence.Get(), fence);
    // ++_fenceValue;

    // if (_fence->GetCompletedValue() < fence) {
    //     _fence->SetEventOnCompletion(fence, _fenceEvent);
    //     WaitForSingleObject(_fenceEvent, INFINITE);
    // }

    // _renderTargetIndex = _swapChain->GetCurrentBackBufferIndex();
}

std::shared_ptr<Pipeline> DirectXGraphicsDriver::CreatePipeline(std::shared_ptr<Shader> shader)
{
    return nullptr;
}

std::shared_ptr<Texture> DirectXGraphicsDriver::CreateTexture()
{
    return nullptr;
}

std::shared_ptr<Shader> DirectXGraphicsDriver::CreateShader()
{
    return std::shared_ptr<Shader>(New DirectXShader());
}

std::unique_ptr<Primitive> DirectXGraphicsDriver::CreatePrimitive()
{
    return nullptr;
    // return std::unique_ptr<Mesh>(New DirectXPrimitive());
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

bool DirectXGraphicsDriver::IsDeviceSuitable(IDXGIAdapter1 * adapter)
{
    DXGI_ADAPTER_DESC1 adapterDesc;
    adapter->GetDesc1(&adapterDesc);

    // Skip the Basic Render Driver adapter
    if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
        return false;
    }

    return true;
}

bool DirectXGraphicsDriver::InitWindow()
{
    HINSTANCE instance = GetModuleHandle(nullptr);

    const char * WINDOW_CLASS_NAME = "DuskWindowClass";

    WNDCLASSEX wndClass = {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = &WndProc,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = instance,
        .hIcon = LoadIcon(nullptr, IDI_APPLICATION),
        .hCursor = LoadCursor(nullptr, IDC_ARROW),
        .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
        .lpszMenuName = nullptr,
        .lpszClassName = WINDOW_CLASS_NAME,
        .hIconSm = nullptr,
    };

    if (!RegisterClassEx(&wndClass)) {
        DuskLogError("RegisterClassEx() failed: %d", GetLastError());
        return false;
    }
    
    std::string name = GetApplicationName();

    std::string title = name + " (" + GetApplicationVersion().GetString() + ")";

    DWORD dwStyle = WS_OVERLAPPEDWINDOW;

    RECT wr = { 0, 0, 640, 480 };
    AdjustWindowRect(&wr, dwStyle, false);

    _window = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        WINDOW_CLASS_NAME,
        title.c_str(),
        dwStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wr.right - wr.left,
        wr.bottom - wr.top,
        nullptr,
        nullptr,
        instance,
        nullptr
    );

    if (!_window) {
        DuskLogError("CreateWindowEx() failed");
        return false;
    }

    ShowWindow(_window, SW_SHOWNORMAL);
    UpdateWindow(_window);

    return true;
}

void DirectXGraphicsDriver::TermWindow()
{
    if (_window) {
        DestroyWindow(_window);
        _window = nullptr;
    }
}

bool DirectXGraphicsDriver::InitDevice()
{
    HRESULT hResult;

    hResult = CreateDXGIFactory2(0, IID_PPV_ARGS(&_dxFactory));
    if (FAILED(hResult)) {
        DuskLogError("CreateDXGIFactory2() failed: %d", GetLastError());
        return false;
    }

    for (unsigned i = 0; ; ++i) {
        hResult = _dxFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&_dxAdapter));
        if (hResult == DXGI_ERROR_NOT_FOUND) {
            break;
        }

        if (IsDeviceSuitable(_dxAdapter.Get())) {
            hResult = D3D12CreateDevice(_dxAdapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&_dxDevice));
            if (SUCCEEDED(hResult)) {
                break;
            }
        }

        _dxAdapter->Release();
    }

    if (!_dxAdapter) {
        DuskLogError("Failed to find a viable DXGI Adapter");
        return false;
    }

    if (!_dxDevice) {
        DuskLogError("Failed to find a create D3D12 Device");
        return false;
    }
    
    // Disable Alt+Enter fullscreen command
    hResult = _dxFactory->MakeWindowAssociation(_window, DXGI_MWA_NO_ALT_ENTER);
    if (FAILED(hResult)) {
        DuskLogWarn("Failed to disable Alt+Enter");
    }

    return true;
}

void DirectXGraphicsDriver::TermDevice()
{
    _dxDevice.Reset();
    _dxAdapter.Reset();
}

bool DirectXGraphicsDriver::InitAllocator()
{
    HRESULT hResult;

    D3D12MA::ALLOCATOR_DESC allocatorDesc = {
        .Flags = D3D12MA::ALLOCATOR_FLAG_NONE,
        .pDevice = _dxDevice.Get(),
        .PreferredBlockSize = 0,
        .pAllocationCallbacks = nullptr,
    };

    hResult = D3D12MA::CreateAllocator(&allocatorDesc, &_dmaAllocator);

    return true;
}

void DirectXGraphicsDriver::TermAllocator()
{
    _dmaAllocator->Release();
    _dmaAllocator = nullptr;
}

bool DirectXGraphicsDriver::InitSwapChain()
{
    // DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { };
    // swapChainDesc.BufferCount = BUFFER_COUNT;
    // swapChainDesc.Width = 640;
    // swapChainDesc.Height = 480;
    // swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    // swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    // swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    // swapChainDesc.SampleDesc.Count = 1;

    // ComPtr<IDXGISwapChain1> swapChain;
    // result = factory->CreateSwapChainForHwnd(
    //     _commandQueue.Get(),
    //     _window,
    //     &swapChainDesc,
    //     nullptr,
    //     nullptr,
    //     &swapChain
    // );
    // if (FAILED(result)) {
    //     DuskLogFatal("Failed to create swap chain");
    // }

    // // Convert the IDXGISwapChain1 into IDXGISwapChain4
    // swapChain.As(&_swapChain);

    return true;
}

void DirectXGraphicsDriver::TermSwapChain()
{

}

bool DirectXGraphicsDriver::ResetSwapChain()
{

    return true;
}

bool DirectXGraphicsDriver::InitDescriptorPool()
{
    // D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = { };
    // rtvHeapDesc.NumDescriptors = BUFFER_COUNT;
    // rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    // rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    // result = _device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_rtvHeap));
    // if (FAILED(result)) {
    //     DuskLogFatal("Failed to create RTV descriptor heap");
    // }

    return true;
}

bool DirectXGraphicsDriver::InitGraphicsPipelines()
{

    return true;
}

bool DirectXGraphicsDriver::InitFramebuffers()
{
    // _renderTargetIndex = _swapChain->GetCurrentBackBufferIndex();

    // _rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());

    // for (unsigned i = 0; i < BUFFER_COUNT; ++i) {
    //     result = _swapChain->GetBuffer(i, IID_PPV_ARGS(&_renderTargets[i]));
    //     if (FAILED(result)) {
    //         DuskLogFatal("Failed to get buffer %d from swap chain", i);
    //     }

    //     _device->CreateRenderTargetView(_renderTargets[i].Get(), nullptr, rtvHandle);
    //     rtvHandle.Offset(1, _rtvDescriptorSize);
    // }

    // result = _device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_commandAllocator));
    // if (FAILED(result)) {
    //     DuskLogFatal("Failed to create command allocator");
    // }

    return true;
}

bool DirectXGraphicsDriver::InitCommandLists()
{
    // _device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocator.Get(), nullptr, IID_PPV_ARGS(&_commandList));
    // _commandList->Close();

    return true;
}

bool DirectXGraphicsDriver::InitSyncObjects()
{
    // _device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
    // _fenceValue = 1;

    // _fenceEvent = CreateEvent(nullptr, false, false, nullptr);

    return true;
}

} // namespace Dusk
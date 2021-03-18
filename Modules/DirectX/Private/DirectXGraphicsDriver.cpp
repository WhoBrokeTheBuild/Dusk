#include <Dusk/DirectX/DirectXGraphicsDriver.hpp>

#include <Dusk/Dusk.hpp>
#include <Dusk/Log.hpp>

// DirectX 12 Extension Library
#include "d3dx12.h"

namespace Dusk {

LRESULT CALLBACK WndProc(HWND hwnd, unsigned msg, WPARAM wParam, LPARAM lParam)
{
    auto gfx = DirectXGraphicsDriver::GetInstance();
    assert(gfx);

    return gfx->ProcessMessage(hwnd, msg, wParam, lParam);
}

DUSK_DIRECTX_API
bool DirectXGraphicsDriver::Initialize()
{
    if (!GraphicsDriver::Initialize()) {
        return false;
    }
    
    if (!InitWindow()) {
        return false;
    }

    if (!InitDevice()) {
        return false;
    }

    #if defined(DUSK_BUILD_DEBUG)

        InitDebugLayer();
        
    #endif

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
    TermAllocator();
    TermDevice();
    TermWindow();
}

void DirectXGraphicsDriver::SetWindowTitle(string_view title)
{
    SetWindowText(_window, title.c_str());
}

string DirectXGraphicsDriver::GetWindowTitle()
{
    char buffer[256];
    GetWindowText(_window, buffer, sizeof(buffer));
    return string(buffer);
}

void DirectXGraphicsDriver::SetWindowSize(const ivec2& size)
{
    // TODO: Check if we need AdjustWindowRect
    SetWindowPos(_window, HWND_TOP, 0, 0, size.x, size.y, SWP_NOMOVE | SWP_NOREPOSITION);
    _windowSize = size;
}

ivec2 DirectXGraphicsDriver::GetWindowSize()
{
    return _windowSize;
}

void DirectXGraphicsDriver::ProcessEvents()
{
    MSG msg;
    if (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void DirectXGraphicsDriver::Render()
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

std::shared_ptr<Mesh> DirectXGraphicsDriver::CreateMesh()
{
    return nullptr;
}

std::shared_ptr<Shader> DirectXGraphicsDriver::CreateShader()
{
    return std::shared_ptr<Shader>(New DirectXShader());
}

std::shared_ptr<Primitive> DirectXGraphicsDriver::CreatePrimitive()
{
    return nullptr;
    // return std::unique_ptr<Mesh>(New DirectXPrimitive());
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
    case WM_SIZE:
        _windowSize = { LOWORD(lParam), HIWORD(lParam) };

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
        fmt::windows_error msg(HRESULT_FROM_WIN32(GetLastError()));
        LogError(DUSK_ANCHOR, "RegisterClassEx() failed: {}", msg);
        return false;
    }
    
    string name = GetApplicationName();

    string title = name + " (" + GetApplicationVersion().GetString() + ")";

    DWORD dwStyle = WS_OVERLAPPEDWINDOW;

    _windowSize = { 640, 480 };

    RECT wr = { 0, 0, _windowSize.x, _windowSize.y };
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
        fmt::windows_error msg(HRESULT_FROM_WIN32(GetLastError()));
        LogError(DUSK_ANCHOR, "CreateWindowEx() failed: {}", msg);
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
        fmt::windows_error msg(hResult);
        LogError(DUSK_ANCHOR, "CreateDXGIFactory2() failed: {}", msg);
        return false;
    }

    for (unsigned i = 0; ; ++i) {
        hResult = _dxFactory->EnumAdapterByGpuPreference(
            i,
            DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
            IID_PPV_ARGS(&_dxAdapter)
        );

        if (hResult == DXGI_ERROR_NOT_FOUND) {
            break;
        }

        if (IsDeviceSuitable(_dxAdapter.Get())) {
            hResult = D3D12CreateDevice(
                _dxAdapter.Get(),
                D3D_FEATURE_LEVEL_12_1,
                IID_PPV_ARGS(&_dxDevice)
            );

            if (SUCCEEDED(hResult)) {
                break;
            }
        }

        _dxAdapter->Release();
    }

    if (!_dxAdapter) {
        LogError(DUSK_ANCHOR, "Failed to find a viable DXGI Adapter");
        return false;
    }

    if (!_dxDevice) {
        LogError(DUSK_ANCHOR, "Failed to find a create D3D12 Device");
        return false;
    }
    
    // Disable Alt+Enter fullscreen command
    hResult = _dxFactory->MakeWindowAssociation(_window, DXGI_MWA_NO_ALT_ENTER);
    if (FAILED(hResult)) {
        fmt::windows_error msg(hResult);
        LogWarning(DUSK_ANCHOR, "MakeWindowAssociation() failed: {}", msg);
    }
    
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {
        .Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
        .Priority = 0,
        .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
        .NodeMask = 0,
    };

    hResult = _dxDevice->CreateCommandQueue(
        &commandQueueDesc,
        IID_PPV_ARGS(&_dxCommandQueue)
    );

    if (FAILED(hResult)) {
        fmt::windows_error msg(hResult);
        LogError(DUSK_ANCHOR, "CreateCommandQueue() failed: {}", msg);
        return false;
    }

    return true;
}

void DirectXGraphicsDriver::TermDevice()
{
    _dxCommandQueue.Reset();
    _dxDevice.Reset();
    _dxAdapter.Reset();
}

bool DirectXGraphicsDriver::InitDebugLayer()
{
    HRESULT hResult;

    hResult = D3D12GetDebugInterface(IID_PPV_ARGS(&_dxDebug));
    if (FAILED(hResult)) {
        fmt::windows_error msg(hResult);
        LogError(DUSK_ANCHOR, "D3D12GetDebugInterface() failed: {}", msg);
        return false;
    }

    _dxDebug->EnableDebugLayer();

    return true;
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

    switch (_dmaAllocator->GetD3D12Options().ResourceHeapTier) {
    case D3D12_RESOURCE_HEAP_TIER_1:
        LogVerbose(DUSK_ANCHOR, "Resource Heap Tier: D3D12_RESOURCE_HEAP_TIER_1");
        break;
    case D3D12_RESOURCE_HEAP_TIER_2:
        LogVerbose(DUSK_ANCHOR, "Resource Heap Tier: D3D12_RESOURCE_HEAP_TIER_2");
        break;
    }

    return true;
}

void DirectXGraphicsDriver::TermAllocator()
{
    _dmaAllocator->Release();
    _dmaAllocator = nullptr;
}

bool DirectXGraphicsDriver::InitSwapChain()
{
    HRESULT hResult;

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {
        .Width = static_cast<UINT>(_windowSize.x),
        .Height = static_cast<UINT>(_windowSize.y),
        .Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        .Stereo = false,
        .SampleDesc = {
            .Count = 1,
            .Quality = 0,
        },
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = BUFFER_COUNT,
        .Scaling = DXGI_SCALING_STRETCH,
        .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
        .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
        .Flags = 0,
    };

    ComPtr<IDXGISwapChain1> swapChain;
    hResult = _dxFactory->CreateSwapChainForHwnd(
        _dxCommandQueue.Get(),
        _window,
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain
    );

    if (FAILED(hResult)) {
        fmt::windows_error msg(hResult);
        LogError(DUSK_ANCHOR, "CreateSwapChainForHwnd() failed: {}", msg);
        return false;
    }

    hResult = swapChain.As(&_dxSwapChain);
    if (FAILED(hResult)) {
        fmt::windows_error msg(hResult);
        LogError(DUSK_ANCHOR, "Failed to convert IDXGISwapChain1 to IDXGISwapChain4: {}", msg);
        return false;
    }

    return true;
}

void DirectXGraphicsDriver::TermSwapChain()
{
    _dxSwapChain.Reset();

}

bool DirectXGraphicsDriver::ResetSwapChain()
{

    return true;
}

bool DirectXGraphicsDriver::InitDescriptorHeaps()
{
    HRESULT hResult;

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        .NumDescriptors = BUFFER_COUNT,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
        .NodeMask = 0,
    };

    hResult = _dxDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_dxRenderTargetViewHeap));
    if (FAILED(hResult)) {
        fmt::windows_error msg(hResult);
        LogError(DUSK_ANCHOR, "CreateDescriptorHeap() failed: {}", msg);
        return false;
    }

    _dxRenderTargetViewDescriptorSize = 
        _dxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    return true;
}

bool DirectXGraphicsDriver::InitRenderTargets()
{
    HRESULT hResult;

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
        _dxRenderTargetViewHeap->GetCPUDescriptorHandleForHeapStart()
    );

    _dxRenderTargets.resize(BUFFER_COUNT);
    _dxCommandAllocators.resize(BUFFER_COUNT);

    for (unsigned i = 0; i < BUFFER_COUNT; ++i) {
        hResult = _dxSwapChain->GetBuffer(i, IID_PPV_ARGS(&_dxRenderTargets[i]));
        if (FAILED(hResult)) {
            fmt::windows_error msg(hResult);
            LogError(DUSK_ANCHOR, "GetBuffer() failed: {}", msg);
            return false;
        }

        D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {
        };

        _dxDevice->CreateRenderTargetView(
            _dxRenderTargets[i].Get(),
            &renderTargetViewDesc,
            rtvHandle
        );

        rtvHandle.Offset(1);

        hResult = _dxDevice->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            IID_PPV_ARGS(&_dxCommandAllocators[i])
        );

        if(FAILED(hResult)) {
            fmt::windows_error msg(hResult);
            LogError(DUSK_ANCHOR, "CreateCommandAllocator() failed: {}", msg);
            return false;
        }
    }

    return true;
}

bool DirectXGraphicsDriver::InitGraphicsPipelines()
{

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
    HRESULT hResult;

    _dxFenceValues.resize(BUFFER_COUNT);

    hResult = _dxDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_dxFence));
    if (FAILED(hResult)) {
        fmt::windows_error msg(hResult);
        LogError(DUSK_ANCHOR, "CreateFence() failed: {}", msg);
        return false;
    }

    ++_dxFenceValues[_renderTargetIndex];

    _dxFenceEvent = CreateEvent(nullptr, false, false, nullptr);
    if (_dxFenceEvent == nullptr) {
        fmt::windows_error msg(HRESULT_FROM_WIN32(GetLastError()));
        LogError(DUSK_ANCHOR, "CreateEvent() failed: {}", msg);
        return false;
    }

    // WaitForGPU

    return true;
}

} // namespace Dusk
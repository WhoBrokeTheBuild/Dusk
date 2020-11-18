#ifndef DUSK_DIRECTX_GRAPHICS_DRIVER_HPP
#define DUSK_DIRECTX_GRAPHICS_DRIVER_HPP

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

#include <Dusk/DirectX/Config.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <shellapi.h>

#define FRAME_COUNT 2

namespace Dusk::DirectX {

using Microsoft::WRL::ComPtr;

class DUSK_DIRECTX_API GraphicsDriver : public Dusk::GraphicsDriver
{
public:

    explicit GraphicsDriver();

    virtual ~GraphicsDriver();

    void SetWindowTitle(const std::string& title) override;

    std::string GetWindowTitle() override;

    void SetWindowSize(const ivec2& size) override;

    ivec2 GetWindowSize() override;

    void ProcessEvents() override;

    void SwapBuffers() override;

    std::unique_ptr<Dusk::Texture> CreateTexture() override;

    std::unique_ptr<Dusk::Shader> CreateShader() override;

    std::unique_ptr<Dusk::Mesh> CreateMesh() override;

    bool SetShaderData(unsigned binding, size_t size, void * data) override;

    LRESULT ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:

    HWND _window = nullptr;

    ComPtr<ID3D12Device> _device = nullptr;
    ComPtr<ID3D12CommandQueue> _commandQueue = nullptr;
    ComPtr<IDXGISwapChain3> _swapChain = nullptr;

    int _frameIndex = 0;

    ComPtr<ID3D12DescriptorHeap> _rtvHeap = nullptr;

    unsigned _rtvDescriptorSize = 0;

    ComPtr<ID3D12Resource> _renderTargets[FRAME_COUNT] = { nullptr };
    ComPtr<ID3D12CommandAllocator> _commandAllocator = nullptr;

    ComPtr<ID3D12PipelineState> _pipelineState = nullptr;
    ComPtr<ID3D12GraphicsCommandList> _commandList = nullptr;

    ComPtr<ID3D12Fence> _fence;
    HANDLE _fenceEvent;
    unsigned long _fenceValue;


}; // class GraphicsDriver

} // namespace Dusk::DirectX

#endif // DUSK_DIRECTX_GRAPHICS_DRIVER_HPP
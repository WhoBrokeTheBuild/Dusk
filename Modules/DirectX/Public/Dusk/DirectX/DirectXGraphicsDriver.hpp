#ifndef DUSK_DIRECTX_GRAPHICS_DRIVER_HPP
#define DUSK_DIRECTX_GRAPHICS_DRIVER_HPP

#include <Dusk/DirectX/DirectXConfig.hpp>

#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/DirectX/DirectXShader.hpp>

#include <D3D12MemAlloc.h>

#define BUFFER_COUNT 2

namespace Dusk {

#define DUSK_DIRECTX_GRAPHICS_DRIVER(x) (dynamic_cast<Dusk::DirectXGraphicsDriver *>(x))

class DUSK_DIRECTX_API DirectXGraphicsDriver : public GraphicsDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(DirectXGraphicsDriver)

    DirectXGraphicsDriver() = default;

    virtual ~DirectXGraphicsDriver() = default;

    bool Initialize() override;

    void Terminate() override;

    void SetWindowTitle(const std::string& title) override;

    std::string GetWindowTitle() override;

    void SetWindowSize(const ivec2& size) override;

    ivec2 GetWindowSize() override;

    void ProcessEvents() override;

    void SwapBuffers() override;

    std::shared_ptr<Pipeline> CreatePipeline(std::shared_ptr<Shader> shader) override;

    std::shared_ptr<Texture> CreateTexture() override;

    std::shared_ptr<Shader> CreateShader() override;

    std::unique_ptr<Primitive> CreatePrimitive() override;

    bool SetShaderData(const std::string& name, size_t size, void * data) override;

    LRESULT ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    ID3D12Device2 * GetDevice() const {
        return _dxDevice.Get();
    }

    D3D12MA::Allocator * GetAllocator() const {
        return _dmaAllocator;
    }

private:

    bool IsDeviceSuitable(IDXGIAdapter1 * adapter);

    bool InitWindow();

    void TermWindow();

    bool InitDevice();

    void TermDevice();

    bool InitAllocator();

    void TermAllocator();
    
    bool InitSwapChain();

    void TermSwapChain();

    bool ResetSwapChain();

    bool InitDescriptorPool();

    bool InitGraphicsPipelines();

    bool InitFramebuffers();

    bool InitCommandLists();

    bool InitSyncObjects();

    HWND _window = nullptr;

    RECT _windowRect;

    int _renderTargetIndex = 0;

    // DirectX 12 Control

    ComPtr<IDXGIFactory6> _dxFactory;

    ComPtr<IDXGIAdapter1> _dxAdapter;

    ComPtr<ID3D12Device2> _dxDevice;

    D3D12MA::Allocator * _dmaAllocator = nullptr;

    ComPtr<ID3D12CommandQueue> _commandQueue;

    ComPtr<IDXGISwapChain4> _swapChain;

    ComPtr<ID3D12Resource> _renderTargets[BUFFER_COUNT] = { nullptr };

    ComPtr<ID3D12CommandAllocator> _commandAllocator;

    ComPtr<ID3D12PipelineState> _pipelineState;

    ComPtr<ID3D12GraphicsCommandList> _commandList;





    ComPtr<ID3D12Fence> _fence;
    HANDLE _fenceEvent;
    unsigned long _fenceValue;

    // Render Target Views

    ComPtr<ID3D12DescriptorHeap> _rtvHeap = nullptr;

    unsigned _rtvDescriptorSize = 0;

}; // class DirectXGraphicsDriver

} // namespace Dusk

#endif // DUSK_DIRECTX_GRAPHICS_DRIVER_HPP
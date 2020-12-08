#ifndef DUSK_DIRECTX_GRAPHICS_DRIVER_HPP
#define DUSK_DIRECTX_GRAPHICS_DRIVER_HPP

#include <Dusk/DirectX/Config.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>
#include <Dusk/DirectX/Shader.hpp>
#include <Dusk/DirectX/Mesh.hpp>

#define BUFFER_COUNT 2

namespace Dusk {

#define DUSK_DIRECTX_GRAPHICS_DRIVER(x) (dynamic_cast<Dusk::DirectXGraphicsDriver *>(x))

class DUSK_DIRECTX_API DirectXGraphicsDriver : public GraphicsDriver
{
public:

    DirectXGraphicsDriver();

    virtual ~DirectXGraphicsDriver();

    inline std::string GetClassID() const override {
        return "Dusk::DirectXUpdateContext";
    }

    void SetWindowTitle(const std::string& title) override;

    std::string GetWindowTitle() override;

    void SetWindowSize(const ivec2& size) override;

    ivec2 GetWindowSize() override;

    void ProcessEvents() override;

    void SwapBuffers() override;

    std::shared_ptr<Pipeline> CreatePipeline() override;

    std::shared_ptr<Texture> CreateTexture() override;

    std::shared_ptr<Shader> CreateShader() override;

    std::shared_ptr<Mesh> CreateMesh() override;

    bool SetShaderData(const std::string& name, size_t size, void * data) override;

    LRESULT ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    ID3D12Device2 * GetDevice() const {
        return _device.Get();
    }

private:

    HWND _window = nullptr;

    RECT _windowRect;

    int _renderTargetIndex = 0;

    // DirectX 12 Control

    ComPtr<ID3D12Device2> _device = nullptr;

    ComPtr<ID3D12CommandQueue> _commandQueue = nullptr;

    ComPtr<IDXGISwapChain4> _swapChain = nullptr;

    ComPtr<ID3D12Resource> _renderTargets[BUFFER_COUNT] = { nullptr };

    ComPtr<ID3D12CommandAllocator> _commandAllocator = nullptr;

    ComPtr<ID3D12PipelineState> _pipelineState = nullptr;

    ComPtr<ID3D12GraphicsCommandList> _commandList = nullptr;





    ComPtr<ID3D12Fence> _fence;
    HANDLE _fenceEvent;
    unsigned long _fenceValue;

    // Render Target Views

    ComPtr<ID3D12DescriptorHeap> _rtvHeap = nullptr;

    unsigned _rtvDescriptorSize = 0;

}; // class DirectXGraphicsDriver

} // namespace Dusk

#endif // DUSK_DIRECTX_GRAPHICS_DRIVER_HPP
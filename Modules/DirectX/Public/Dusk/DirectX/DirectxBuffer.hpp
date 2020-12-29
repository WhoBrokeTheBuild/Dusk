#ifndef DUSK_DIRECTX_BUFFER_HPP
#define DUSK_DIRECTX_BUFFER_HPP

#include <Dusk/DirectX/DirectXConfig.hpp>
#include <Dusk/Buffer.hpp>

#include <D3D12MemAlloc.h>

#include <optional>

namespace Dusk::DirectX {

class DUSK_DIRECTX_API DirectXBuffer : public Buffer
{
public:

    DISALLOW_COPY_AND_ASSIGN(DirectXBuffer)

    DirectXBuffer() = default;

    virtual ~DirectXBuffer() = default;

    bool Initialize(size_t size, uint8_t * data, BufferUsage bufferUsage, MemoryUsage memoryUsage) override;

    void Terminate() override;

    bool ReadFrom(size_t offset, size_t length, uint8_t * data) override;

    bool WriteTo(size_t offset, size_t length, uint8_t * data) override;

private:

    size_t _size;

    ComPtr<ID3D12Resource> _dxResource;

    D3D12MA::Allocation * _dmaAllocation = nullptr;

}; // class DirectXBuffer

inline std::optional<D3D12_RESOURCE_STATES> GetDXResourceState(BufferUsage bufferUsage)
{
    switch (bufferUsage) {
    case BufferUsage::Index:
        return D3D12_RESOURCE_STATE_INDEX_BUFFER;
    case BufferUsage::Vertex:
    case BufferUsage::Constant:
        return D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    }

    return {};
}

inline std::optional<D3D12_HEAP_TYPE> GetDXHeapType(MemoryUsage memoryUsage)
{
    switch (memoryUsage) {
    case MemoryUsage::GPU:
        return D3D12_HEAP_TYPE_DEFAULT;
    case MemoryUsage::UploadOnce:
    case MemoryUsage::UploadOften:
        return D3D12_HEAP_TYPE_UPLOAD;
    case MemoryUsage::Download:
        return D3D12_HEAP_TYPE_READBACK;
    }

    return {};
}

} // Dusk::DirectX

#endif // DUSK_DIRECTX_BUFFER_HPP
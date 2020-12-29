#include <Dusk/DirectX/DirectXBuffer.hpp>

#include <Dusk/DirectX/DirectXGraphicsDriver.hpp>

namespace Dusk::DirectX {

DUSK_DIRECTX_API
bool DirectXBuffer::Initialize(size_t size, uint8_t * data, BufferUsage bufferUsage, MemoryUsage memoryUsage)
{
    HRESULT hResult;

    _bufferUsage = bufferUsage;
    _memoryUsage = memoryUsage;
    _size = size;

    if (!data && _memoryUsage == MemoryUsage::GPU) {
        DuskLogError("Attempting to create an empty buffer with MemoryUsage GPU");
        return false;
    }

    DirectXGraphicsDriver * gfx = DUSK_DIRECTX_GRAPHICS_DRIVER(GetGraphicsDriver());

    auto resourceState = GetDXResourceState(_bufferUsage);
    auto heapType = GetDXHeapType(_memoryUsage);

    if (!resourceState || !heapType) {
        return false;
    }

    D3D12MA::ALLOCATION_DESC allocationDesc = {
        .HeapType = heapType.value(),
    };

    D3D12_RESOURCE_DESC resourceDesc = {
        .Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
        .Alignment = 0,
        .Width = size,
        .Height = 1,
        .DepthOrArraySize = 1,
        .MipLevels = 1,
        .Format = DXGI_FORMAT_UNKNOWN,
        .SampleDesc = {
            .Count = 1,
            .Quality = 0,
        },
        .Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR, // TODO: Investigate
        .Flags = D3D12_RESOURCE_FLAG_NONE,
    };

    D3D12_CLEAR_VALUE clearValue = { };

    hResult = gfx->GetAllocator()->CreateResource(
        &allocationDesc,
        &resourceDesc,
        resourceState.value(),
        &clearValue,
        &_dmaAllocation,
        IID_PPV_ARGS(&_dxResource)
    );

    if (FAILED(hResult)) {
        WindowsErrorMessage msg(hResult);
        DuskLogError("CreateResource() failed: %s", msg);
        return false;
    }

    D3D12_RANGE readRange = { 0, 0 };

    if (data) {
        void * ptr;
        hResult = _dxResource->Map(0, &readRange, &ptr);
        if (FAILED(hResult)) {
            WindowsErrorMessage msg(hResult);
            DuskLogError("Map() failed: %s", msg);
            return false;
        }

        memcpy(ptr, data, _size);

        _dxResource->Unmap(0, &readRange);
    }

    return true;
}

DUSK_DIRECTX_API
void DirectXBuffer::Terminate()
{
    _dmaAllocation->Release();
    _dmaAllocation = nullptr;

    _dxResource.Reset();
}

DUSK_OPENGL_API
bool DirectXBuffer::ReadFrom(size_t offset, size_t length, uint8_t * data)
{
    if (_memoryUsage != MemoryUsage::Download) {
        DuskLogError("Unable to read data from buffer with MemoryUsage: %s",
            MemoryUsageToString(_memoryUsage));
        return false;
    }

    D3D12_RANGE readRange = { offset, offset + length };

    void * ptr;
    hResult = _dxResource->Map(0, &readRange, &ptr);
    if (FAILED(hResult)) {
        WindowsErrorMessage msg(hResult);
        DuskLogError("Map() failed: %s", msg);
        return false;
    }

    memcpy(data, ptr, _size);

    _dxResource->Unmap(0, &readRange);

    return true;
}

DUSK_OPENGL_API
bool DirectXBuffer::WriteTo(size_t offset, size_t length, uint8_t * data)
{
    if (_memoryUsage != MemoryUsage::UploadOnce && _memoryUsage != MemoryUsage::UploadOften) {
        DuskLogError("Unable to write data to buffer with MemoryUsage: %s",
            MemoryUsageToString(_memoryUsage));
        return false;
    }

    D3D12_RANGE readRange = { offset, offset + length };

    void * ptr;
    hResult = _dxResource->Map(0, &readRange, &ptr);
    if (FAILED(hResult)) {
        WindowsErrorMessage msg(hResult);
        DuskLogError("Map() failed: %s", msg);
        return false;
    }

    memcpy(ptr, data, _size);

    _dxResource->Unmap(0, &readRange);

    return true;
}

} // namespace Dusk::DirectX
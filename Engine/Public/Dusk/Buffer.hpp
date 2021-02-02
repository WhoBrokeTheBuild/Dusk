#ifndef DUSK_BUFFER_HPP
#define DUSK_BUFFER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/String.hpp>

#include <cstdint>

namespace Dusk {

enum class BufferUsage
{
    Index,
    Vertex,
    Constant,

}; // enum class BufferUsage

enum class MemoryUsage
{
    GPU,
    UploadOnce,
    UploadOften,
    Download,

}; // enum class MemoryUsage

class DUSK_ENGINE_API Buffer : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(Buffer)

    Buffer() = default;

    virtual ~Buffer() = default;

    virtual bool Initialize(size_t size, uint8_t * data, BufferUsage bufferUsage, MemoryUsage memoryUsage) = 0;

    virtual void Terminate() = 0;

    virtual bool ReadFrom(size_t offset, size_t length, uint8_t * data);

    virtual bool WriteTo(size_t offset, size_t length, uint8_t * data);

    virtual bool MapBufferToMemory() = 0;

protected:

    BufferUsage _bufferUsage;

    MemoryUsage _memoryUsage;

    void * _mappedBufferMemory;

}; // class Buffer

inline string BufferUsageToString(BufferUsage bufferUsage)
{
    switch (bufferUsage) {
        case BufferUsage::Index:
            return "Index";
        case BufferUsage::Vertex:
            return "Vertex";
        case BufferUsage::Constant:
            return "Constant";
    }

    return "Unknown";
}

inline string MemoryUsageToString(MemoryUsage memoryUsage)
{
    switch (memoryUsage) {
        case MemoryUsage::GPU:
            return "GPU";
        case MemoryUsage::UploadOnce:
            return "UploadOnce";
        case MemoryUsage::UploadOften:
            return "UploadOften";
        case MemoryUsage::Download:
            return "Download";
    }

    return "Unknown";
}

} // namespace Dusk

#endif // DUSK_BUFFER_HPP
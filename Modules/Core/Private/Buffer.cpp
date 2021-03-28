#include <Dusk/Buffer.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Exception.hpp>

#include <cstring>

namespace Dusk {

DUSK_CORE_API
void Buffer::ReadFrom(size_t offset, size_t length, uint8_t * data)
{
    assert(_memoryUsage == MemoryUsage::Download);
    assert(_mappedBufferMemory);

    memcpy(data, _mappedBufferMemory, length);
}

DUSK_CORE_API
void Buffer::WriteTo(size_t offset, size_t length, uint8_t * data)
{
    assert(_memoryUsage == MemoryUsage::UploadOnce || _memoryUsage == MemoryUsage::UploadOften);
    assert(_mappedBufferMemory);

    memcpy(_mappedBufferMemory, data, length);
}

} // namespace Dusk
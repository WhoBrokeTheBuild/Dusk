#include <Dusk/Buffer.hpp>
#include <Dusk/Log.hpp>

#include <cstring>

namespace Dusk {

DUSK_ENGINE_API
bool Buffer::ReadFrom(size_t offset, size_t length, uint8_t * data)
{
    if (_memoryUsage != MemoryUsage::Download) {
        LogError(DUSK_ANCHOR, "Unable to read data from buffer with MemoryUsage: {}",
            MemoryUsageToString(_memoryUsage));
        return false;
    }

    if (!_mappedBufferMemory) {
        LogError(DUSK_ANCHOR, "Buffer is not mapped for reading");
        return false;
    }

    memcpy(data, _mappedBufferMemory, length);

    return true;
}

DUSK_ENGINE_API
bool Buffer::WriteTo(size_t offset, size_t length, uint8_t * data)
{
    if (_memoryUsage != MemoryUsage::UploadOnce && _memoryUsage != MemoryUsage::UploadOften) {
        LogError(DUSK_ANCHOR, "Unable to write data to buffer with MemoryUsage: {}",
            MemoryUsageToString(_memoryUsage));
        return false;
    }

    if (!_mappedBufferMemory) {
        LogError(DUSK_ANCHOR, "Buffer is not mapped for writing");
        return false;
    }

    memcpy(_mappedBufferMemory, data, length);

    return true;
}

} // namespace Dusk
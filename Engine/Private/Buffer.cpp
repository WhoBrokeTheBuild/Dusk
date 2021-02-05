#include <Dusk/Buffer.hpp>
#include <Dusk/Log.hpp>

#include <cstring>

namespace Dusk {

DUSK_ENGINE_API
bool Buffer::ReadFrom(size_t offset, size_t length, uint8_t * data)
{
    if (_memoryUsage != MemoryUsage::Download) {
        DuskLogError("Unable to read data from buffer with MemoryUsage: %s",
            MemoryUsageToString(_memoryUsage));
        return false;
    }

    if (!_mappedBufferMemory) {
        DuskLogError("Buffer is not mapped for reading");
        return false;
    }

    memcpy(data, _mappedBufferMemory, length);

    return true;
}

DUSK_ENGINE_API
bool Buffer::WriteTo(size_t offset, size_t length, uint8_t * data)
{
    if (_memoryUsage != MemoryUsage::UploadOnce && _memoryUsage != MemoryUsage::UploadOften) {
        DuskLogError("Unable to write data to buffer with MemoryUsage: %s",
            MemoryUsageToString(_memoryUsage));
        return false;
    }

    if (!_mappedBufferMemory) {
        DuskLogError("Buffer is not mapped for writing");
        return false;
    }

    memcpy(_mappedBufferMemory, data, length);

    return true;
}

} // namespace Dusk
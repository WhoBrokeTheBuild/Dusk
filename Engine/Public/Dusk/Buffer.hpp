#ifndef DUSK_BUFFER_HPP
#define DUSK_BUFFER_HPP

#include <Dusk/Config.hpp>

#include <cstdint>
#include <string>

namespace Dusk {

enum class BufferUsage
{
    Index,
    Vertex,
    Constant,

}; // enum class BufferUsage

class DUSK_ENGINE_API Buffer
{
public:

    DISALLOW_COPY_AND_ASSIGN(Buffer)

    Buffer() = default;

    virtual ~Buffer() = default;

    virtual bool Initialize(size_t size, uint8_t * data, BufferUsage usage) = 0;

    virtual void Terminate() = 0;

}; // class Buffer

inline std::string BufferUsageToString(BufferUsage bufferUsage)
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

} // namespace Dusk

#endif // DUSK_BUFFER_HPP
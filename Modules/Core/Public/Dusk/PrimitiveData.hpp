#ifndef DUSK_PRIMITIVE_DATA_HPP
#define DUSK_PRIMITIVE_DATA_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Vertex.hpp>
#include <Dusk/Material.hpp>

#include <span>
#include <memory>

namespace Dusk {

enum class PrimitiveTopology 
{
    PointList,
    LineList,
    LineStrip,
    TriangleList,
    TriangleStrip,

}; // enum class PrimitiveTopology

class DUSK_CORE_API PrimitiveData : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(PrimitiveData);

    PrimitiveData() = default;

    virtual ~PrimitiveData() = default;

    virtual PrimitiveTopology GetTopology() const = 0;

    virtual std::span<uint32_t> GetIndexList() {
        return std::span<uint32_t>();
    }
    
    virtual std::span<Vertex> GetVertexList() = 0;

    virtual std::shared_ptr<Material> GetMaterial() {
        return nullptr;
    }

    virtual void CalculateTangents();

}; // class PrimitiveData

inline string PrimitiveTopologyToString(PrimitiveTopology primitiveTopology)
{
    switch (primitiveTopology) {
    case PrimitiveTopology::PointList:
        return "PointList";
    case PrimitiveTopology::LineList:
        return "LineList";
    case PrimitiveTopology::LineStrip:
        return "LineStrip";
    case PrimitiveTopology::TriangleList:
        return "TriangleList";
    case PrimitiveTopology::TriangleStrip:
        return "TriangleStrip";
    }

    return "Unknown";
}

} // namespace Dusk

#endif // DUSK_PRIMITIVE_DATA_HPP
#ifndef DUSK_PRIMITIVE_HPP
#define DUSK_PRIMITIVE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/Vertex.hpp>
#include <Dusk/String.hpp>

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

class PrimitiveData;

class DUSK_ENGINE_API Primitive : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(Primitive)

    Primitive() = default;

    virtual ~Primitive() = default;

    virtual bool Load(const std::unique_ptr<PrimitiveData>& data) = 0;

}; // class Primitive

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

#endif // DUSK_PRIMITIVE_HPP
#ifndef DUSK_PRIMITIVE_HPP
#define DUSK_PRIMITIVE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/Vertex.hpp>

#include <memory>

namespace Dusk {

enum class PrimitiveTopology 
{
    Points,
    Lines,
    LineStrip,
    Triangles,
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

inline std::string PrimitiveTopologyToString(PrimitiveTopology primitiveTopology)
{
    switch (primitiveTopology) {
    case PrimitiveTopology::Points:
        return "Points";
    case PrimitiveTopology::Lines:
        return "Lines";
    case PrimitiveTopology::LineStrip:
        return "LineStrip";
    case PrimitiveTopology::Triangles:
        return "Triangles";
    case PrimitiveTopology::TriangleStrip:
        return "TriangleStrip";
    }

    return "Unknown";
}

} // namespace Dusk

#endif // DUSK_PRIMITIVE_HPP
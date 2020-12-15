#include <Dusk/Primitive.hpp>
#include <Dusk/PrimitiveData.hpp>

#include <Dusk/Log.hpp>

namespace Dusk {

uint32_t Primitive::GetVertexAttributeLocation(VertexAttribute attribute) const
{
    switch (attribute) {
    case VertexAttribute::Position:
        return 0;
    case VertexAttribute::Normal:
        return 1;
    case VertexAttribute::Tangent:
        return 2;
    case VertexAttribute::Bitangent:
        return 3;
    case VertexAttribute::Color:
        return 4;
    case VertexAttribute::TexCoord1:
        return 5;
    case VertexAttribute::TexCoord2:
        return 6;
    case VertexAttribute::Joints:
        return 7;
    case VertexAttribute::Weights:
        return 8;
    }

    DuskLogFatal("Unexpected VertexAttribute: %d", (int)attribute);
}

} // namespace Dusk
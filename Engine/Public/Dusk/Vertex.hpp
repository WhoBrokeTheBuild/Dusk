#ifndef DUSK_VERTEX_HPP
#define DUSK_VERTEX_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/String.hpp>

namespace Dusk {

struct Vertex
{
    vec4 Position;
    vec4 Normal;
    vec4 Tangent;
    
    vec4 Color;

    vec2 TexCoord1;
    vec2 TexCoord2;

    uvec2 Joints;
    vec4 Weights;
    
}; // struct Vertex

enum class VertexAttribute
{
    Position,
    Normal,
    Tangent,
    Color,
    TexCoord1,
    TexCoord2,
    Joints,
    Weights,

}; // enum class VertexAttribute

inline string VertexAttributeToString(VertexAttribute vertexAttribute)
{
    switch (vertexAttribute) {
        case VertexAttribute::Position:
            return "Position";
        case VertexAttribute::Normal:
            return "Normal";
        case VertexAttribute::Tangent:
            return "Tangent";
        case VertexAttribute::Color:
            return "Color";
        case VertexAttribute::TexCoord1:
            return "TexCoord1";
        case VertexAttribute::TexCoord2:
            return "TexCoord2";
        case VertexAttribute::Joints:
            return "Joints";
        case VertexAttribute::Weights:
            return "Weights";
    }

    return "Unknown";
}

inline uint32_t GetVertexAttributeLocation(VertexAttribute attribute)
{
    switch (attribute) {
    case VertexAttribute::Position:
        return 0;
    case VertexAttribute::Normal:
        return 1;
    case VertexAttribute::Tangent:
        return 2;
    case VertexAttribute::Color:
        return 3;
    case VertexAttribute::TexCoord1:
        return 4;
    case VertexAttribute::TexCoord2:
        return 5;
    case VertexAttribute::Joints:
        return 6;
    case VertexAttribute::Weights:
        return 7;
    }

    return UINT32_MAX;
}

} // namespace Dusk

#endif // DUSK_VERTEX_HPP

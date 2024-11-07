#ifndef DUSK_PRIMITIVE_VERTEX_HPP
#define DUSK_PRIMITIVE_VERTEX_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/Span.hpp>

#include <Dusk/ThirdParty/vulkan.hpp>

namespace dusk {

struct DUSK_API PrimitiveVertex
{
    struct AttributeLocation
    {
        static constexpr uint32_t Position  = 0;
        static constexpr uint32_t Normal    = 1;
        static constexpr uint32_t Tangent   = 2;
        static constexpr uint32_t Color     = 3;
        static constexpr uint32_t TexCoord  = 4;
        static constexpr uint32_t Joints    = 5;
        static constexpr uint32_t Weights   = 6;
    };

    alignas(4) vec4 Position;
    
    alignas(4) vec4 Normal;
    
    alignas(4) vec4 Tangent;

    alignas(4) vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

    alignas(4) vec2 TexCoord;
    
    alignas(4) uvec4 Joints;

    alignas(4) vec4 Weights;

}; // struct PrimitiveVertex

static_assert(
    sizeof(PrimitiveVertex) == (
        sizeof(PrimitiveVertex::Position) +
        sizeof(PrimitiveVertex::Normal) + 
        sizeof(PrimitiveVertex::Tangent) +
        sizeof(PrimitiveVertex::Color) +
        sizeof(PrimitiveVertex::TexCoord) +
        sizeof(PrimitiveVertex::Joints) +
        sizeof(PrimitiveVertex::Weights)
    ),
    "sizeof(Vertex) is not tightly packed"
);

} // namespace dusk

#endif // DUSK_PRIMITIVE_VERTEX_HPP
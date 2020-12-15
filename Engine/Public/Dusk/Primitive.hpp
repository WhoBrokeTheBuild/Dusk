#ifndef DUSK_PRIMITIVE_HPP
#define DUSK_PRIMITIVE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Math.hpp>

#include <memory>

namespace Dusk {

class PrimitiveData;

class DUSK_ENGINE_API Primitive : public Object
{
public:

    enum class Topology 
    {
        Points,
        Lines,
        LineStrip,
        Triangles,
        TriangleStrip,

    }; // enum class Topology

    struct Vertex
    {
        glm::vec4 Position;
        glm::vec4 Normal;
        glm::vec4 Tangent;
        glm::vec4 Bitangent;
        
        glm::vec4 Color;

        glm::vec2 TexCoord1;
        glm::vec2 TexCoord2;

        uint16_t Joints[4];
        float Weights[4];
        
    }; // struct Vertex

    enum class VertexAttribute
    {
        Position,
        Normal,
        Tangent,
        Bitangent,
        Color,
        TexCoord1,
        TexCoord2,
        Joints,
        Weights,

    }; // enum class VertexAttribute

    DISALLOW_COPY_AND_ASSIGN(Primitive)

    Primitive() = default;

    virtual ~Primitive() = default;

    virtual bool Load(const std::unique_ptr<PrimitiveData>& data) = 0;

    uint32_t GetVertexAttributeLocation(VertexAttribute attribute) const;

}; // class Primitive

} // namespace Dusk

#endif // DUSK_PRIMITIVE_HPP
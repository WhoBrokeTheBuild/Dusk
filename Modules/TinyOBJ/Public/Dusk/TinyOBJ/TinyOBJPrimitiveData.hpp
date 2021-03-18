#ifndef DUSK_TINYOBJ_PRIMITIVE_DATA_HPP
#define DUSK_TINYOBJ_PRIMITIVE_DATA_HPP

#include <Dusk/TinyOBJ/TinyOBJConfig.hpp>

#include <Dusk/PrimitiveData.hpp>

namespace Dusk::TinyOBJ {

class DUSK_TINYOBJ_API TinyOBJPrimitiveData : public PrimitiveData
{
public:

    std::vector<Vertex> vertexList;

    std::shared_ptr<Material> material;

    PrimitiveTopology GetTopology() const override {
        return PrimitiveTopology::TriangleList;
    }

    std::span<Vertex> GetVertexList() override {
        return vertexList;
    }

    std::shared_ptr<Material> GetMaterial() override {
        return material;
    }

}; // class TinyOBJPrimitiveData

} // namespace Dusk::TinyOBJ

#endif // DUSK_TINYOBJ_PRIMITIVE_DATA_HPP
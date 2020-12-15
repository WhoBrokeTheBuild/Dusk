#ifndef DUSK_TINYOBJ_PRIMITIVE_DATA_HPP
#define DUSK_TINYOBJ_PRIMITIVE_DATA_HPP

#include <Dusk/TinyOBJ/TinyOBJConfig.hpp>

#include <Dusk/PrimitiveData.hpp>

namespace Dusk::TinyOBJ {

class DUSK_TINYOBJ_API TinyOBJPrimitiveData : public PrimitiveData
{
public:

    std::vector<Primitive::Vertex> VertexList;

    Primitive::Topology GetTopology() const override {
        return Primitive::Topology::Triangles;
    }

    gsl::span<Primitive::Vertex> GetVertexList() override {
        return gsl::span<Primitive::Vertex>(VertexList.data(), VertexList.size());
    }

}; // class TinyOBJPrimitiveData

} // namespace Dusk::TinyOBJ

#endif // DUSK_TINYOBJ_PRIMITIVE_DATA_HPP
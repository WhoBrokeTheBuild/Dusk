#ifndef DUSK_GLTF2_PRIMITIVE_DATA_HPP
#define DUSK_GLTF2_PRIMITIVE_DATA_HPP

#include <Dusk/GLTF2/GLTF2Config.hpp>
#include <Dusk/PrimitiveData.hpp>

#include <vector>

namespace Dusk::GLTF2 {

class DUSK_GLTF2_API GLTF2PrimitiveData : public PrimitiveData
{
public:

    DISALLOW_COPY_AND_ASSIGN(GLTF2PrimitiveData)

    GLTF2PrimitiveData() = default;

    virtual ~GLTF2PrimitiveData() = default;

    PrimitiveTopology topology;

    std::vector<uint32_t> indexList;

    std::vector<Vertex> vertexList;

    std::shared_ptr<Material> material;

    PrimitiveTopology GetTopology() const override {
        return topology;
    }

    std::span<uint32_t> GetIndexList() override {
        return indexList;
    }

    std::span<Vertex> GetVertexList() override {
        return vertexList;
    }

    std::shared_ptr<Material> GetMaterial() override {
        return material;
    }

}; // class GLTF2PrimitiveData

} // namespace Dusk::GLTF2

#endif // DUSK_GLTF2_PRIMITIVE_DATA_HPP
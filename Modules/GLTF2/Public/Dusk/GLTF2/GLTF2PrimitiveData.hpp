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

    PrimitiveTopology Topology;

    std::vector<uint32_t> IndexList;

    std::vector<Vertex> VertexList;

    virtual PrimitiveTopology GetTopology() const {
        return Topology;
    }

    virtual gsl::span<uint32_t> GetIndexList() {
        return IndexList;
    }

    virtual gsl::span<Vertex> GetVertexList() {
        return VertexList;
    }

}; // class GLTF2PrimitiveData

} // namespace Dusk::GLTF2

#endif // DUSK_GLTF2_PRIMITIVE_DATA_HPP
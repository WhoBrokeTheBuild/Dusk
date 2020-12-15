#ifndef DUSK_GLTF2_MESH_IMPORTER_HPP
#define DUSK_GLTF2_MESH_IMPORTER_HPP

#include <Dusk/GLTF2/GLTF2Config.hpp>

#include <Dusk/MeshImporter.hpp>

#include <vector>

namespace Dusk::GLTF2 {

class DUSK_GLTF2_API GLTF2PrimitiveData : public PrimitiveData
{
public:

    std::vector<uint32_t> IndexList;

    std::vector<Primitive::Vertex> VertexList;

    Primitive::Topology GetTopology() const override {
        return Primitive::Topology::Triangles;
    }

    gsl::span<uint32_t> GetIndexList() override {
        return IndexList;
    }

    gsl::span<Primitive::Vertex> GetVertexList() override {
        return gsl::span<Primitive::Vertex>(VertexList.data(), VertexList.size());
    }

}; // class PrimitiveData

class DUSK_GLTF2_API GLTF2MeshImporter : public MeshImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(GLTF2MeshImporter)

    GLTF2MeshImporter() = default;

    std::vector<std::unique_ptr<PrimitiveData>> LoadFromFile(const std::string& filename) override;

}; // class GLTF2MeshImporter

} // namespace Dusk::GLTF2

#endif // DUSK_GLTF2_MESH_IMPORTER_HPP
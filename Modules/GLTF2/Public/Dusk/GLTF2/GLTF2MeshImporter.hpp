#ifndef DUSK_GLTF2_MESH_IMPORTER_HPP
#define DUSK_GLTF2_MESH_IMPORTER_HPP

#include <Dusk/GLTF2/GLTF2Config.hpp>

#include <Dusk/MeshImporter.hpp>

#include <vector>

namespace Dusk::GLTF2 {

class DUSK_GLTF2_API GLTF2MeshData : public MeshData
{
public:

    inline std::string GetClassID() const override {
        return "Dusk::GLTF2MeshData";
    }

    std::vector<float> Vertices;

    std::vector<float> Normals;

    std::vector<float> UVs;

    std::vector<float> Colors;

    Mode GetMode() const override {
        return Mode::Triangles;
    }

    gsl::span<const unsigned> GetIndices() const override {
        return gsl::span<unsigned>();
    }

    gsl::span<const float> GetVertices() const override {
        return gsl::span<const float>(Vertices.data(), Vertices.size());
    }

    gsl::span<const float> GetNormals() const override {
        return gsl::span<const float>(Normals.data(), Normals.size());
    }

    gsl::span<const float> GetUVs() const override {
        return gsl::span<const float>(UVs.data(), UVs.size());
    }

    gsl::span<const float> GetColors() const override {
        return gsl::span<const float>(Colors.data(), Colors.size());
    }

    gsl::span<const float> GetTangents() const override {
        return gsl::span<float>();
    }

    gsl::span<const float> GetBitangents() const override {
        return gsl::span<float>();
    }

}; // class MeshData

class DUSK_GLTF2_API GLTF2MeshImporter : public MeshImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(GLTF2MeshImporter)

    GLTF2MeshImporter() = default;

    inline std::string GetClassID() const override {
        return "Dusk::GLTF2MeshImporter";
    }

    std::vector<std::unique_ptr<MeshData>> LoadFromFile(const std::string& filename) override;

}; // class GLTF2MeshImporter

} // namespace Dusk::GLTF2

#endif // DUSK_GLTF2_MESH_IMPORTER_HPP
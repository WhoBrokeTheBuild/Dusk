#ifndef DUSK_TINYOBJ_MESH_IMPORTER_HPP
#define DUSK_TINYOBJ_MESH_IMPORTER_HPP

#include <Dusk/TinyOBJ/Config.hpp>
#include <Dusk/Graphics/MeshImporter.hpp>

#include <vector>

namespace Dusk::TinyOBJ {

class DUSK_TINYOBJ_API MeshData : public Dusk::MeshData
{
public:

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

class DUSK_TINYOBJ_API MeshImporter : public Dusk::MeshImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(MeshImporter)

    MeshImporter() = default;

    inline std::string GetClassID() const override {
        return "Dusk::TinyOBJ::MeshImporter";
    }

    std::vector<std::unique_ptr<Dusk::MeshData>> LoadFromFile(const std::string& filename) override;

}; // class MeshImporter

} // namespace Dusk::TinyOBJ

#endif // DUSK_TINYOBJ_MESH_IMPORTER_HPP
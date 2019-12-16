#ifndef DUSK_TINYOBJ_MESH_IMPORTER_HPP
#define DUSK_TINYOBJ_MESH_IMPORTER_HPP

#include <Dusk/TinyOBJ/Config.hpp>
#include <Dusk/Graphics/MeshImporter.hpp>

#include <vector>

namespace Dusk::TinyOBJ {

class DUSK_TINYOBJ_API MeshData : public IMeshData
{
public:

    std::vector<float> Vertices;

    std::vector<float> Normals;

    std::vector<float> UVs;

    std::vector<float> Colors;

    gsl::span<const unsigned> GetIndices() const override {
        return gsl::span<unsigned>();
    }

    gsl::span<const float> GetVertices() const override {
        return gsl::span(Vertices.data(), Vertices.size());
    }

    gsl::span<const float> GetNormals() const override {
        return gsl::span(Normals.data(), Normals.size());
    }

    gsl::span<const float> GetUVs() const override {
        return gsl::span(UVs.data(), UVs.size());
    }

    gsl::span<const float> GetColors() const override {
        return gsl::span(Colors.data(), Colors.size());
    }

    gsl::span<const float> GetTangents() const override {
        return gsl::span<float>();
    }

    gsl::span<const float> GetBitangents() const override {
        return gsl::span<float>();
    }

}; // class MeshData

class DUSK_TINYOBJ_API MeshImporter : public IMeshImporter
{
public:

    std::vector<std::unique_ptr<IMeshData>> LoadFromFile(const std::string& filename) override;

}; // class MeshImporter

} // namespace Dusk::TinyOBJ

#endif // DUSK_TINYOBJ_MESH_IMPORTER_HPP
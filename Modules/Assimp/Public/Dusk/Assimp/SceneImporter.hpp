#ifndef DUSK_ASSIMP_SCENE_IMPORTER_HPP
#define DUSK_ASSIMP_SCENE_IMPORTER_HPP

#include <Dusk/Assimp/Config.hpp>
#include <Dusk/Scene/SceneImporter.hpp>
#include <Dusk/Graphics/Mesh.hpp>

#include <Dusk/Log.hpp>

#include <vector>

namespace Dusk::Assimp {

class DUSK_ASSIMP_API MeshData : public Dusk::MeshData
{
public:

    std::vector<float> Vertices;

    std::vector<float> Normals;

    std::vector<float> UVs;

    std::vector<float> Colors;

    std::vector<float> Tangents;

    std::vector<float> Bitangents;

    Mode GetMode() const override {
        return Mode::Triangles;
    }

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
        return gsl::span(Tangents.data(), Tangents.size());
    }

    gsl::span<const float> GetBitangents() const override {
        return gsl::span(Bitangents.data(), Bitangents.size());
    }

}; // class MeshData

class DUSK_ASSIMP_API SceneImporter : public ISceneImporter
{
public:

    bool LoadFromFile(Entity * root, const std::string& filename) override;

};

} // namespace Dusk::Assimp

#endif // DUSK_ASSIMP_SCENE_IMPORTER_HPP
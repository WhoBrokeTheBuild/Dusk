#ifndef DUSK_CORE_MESH_HPP
#define DUSK_CORE_MESH_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>

#include <gsl/gsl>

namespace Dusk {

class DUSK_ENGINE_API MeshData : public Object
{
public:

    enum class Mode 
    {
        Points,
        Lines,
        Triangles,
        TriangleFan,
    };

    DISALLOW_COPY_AND_ASSIGN(MeshData);

    MeshData() = default;

    virtual ~MeshData() = default;

    inline std::string GetClassID() const override {
        return "Dusk::UpdateContext";
    }

    virtual Mode GetMode() const = 0;

    virtual gsl::span<const unsigned> GetIndices() const = 0;

    virtual gsl::span<const float> GetVertices() const = 0;

    virtual gsl::span<const float> GetNormals() const = 0;

    virtual gsl::span<const float> GetUVs() const = 0;

    virtual gsl::span<const float> GetColors() const = 0;

    virtual gsl::span<const float> GetTangents() const = 0;

    virtual gsl::span<const float> GetBitangents() const = 0;

}; // class MeshData

class DUSK_ENGINE_API Mesh : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(Mesh)

    Mesh() = default;

    virtual ~Mesh() = default;

    inline std::string GetClassID() const override {
        return "Dusk::Mesh";
    }

    virtual void Render() = 0;

    virtual bool Load(const MeshData * data) = 0;

protected:

    enum class VertexAttributeLocation {
        Position        = 0,
        Normal          = 1,
        Tangent         = 2,
        Bitangent       = 3,
        Color1          = 4,
        Color2          = 5,
        UV1             = 6,
        UV2             = 6,
        UV3             = 7,
        UV4             = 7,
        UV5             = 8,
        UV6             = 8,
    };

}; // class Mesh

} // namespace Dusk

#endif // DUSK_CORE_MESH_HPP
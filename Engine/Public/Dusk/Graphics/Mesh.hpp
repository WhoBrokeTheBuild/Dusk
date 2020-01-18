#ifndef DUSK_CORE_MESH_HPP
#define DUSK_CORE_MESH_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>

#include <gsl/gsl>

namespace Dusk {

class DUSK_CORE_API IMeshData
{
public:

    enum class Mode 
    {
        Points,
        Lines,
        Triangles,
        TriangleFan,
    };

    DISALLOW_COPY_AND_ASSIGN(IMeshData);

    explicit IMeshData() = default;

    virtual ~IMeshData() = default;

    virtual Mode GetMode() const = 0;

    virtual gsl::span<const unsigned> GetIndices() const = 0;

    virtual gsl::span<const float> GetVertices() const = 0;

    virtual gsl::span<const float> GetNormals() const = 0;

    virtual gsl::span<const float> GetUVs() const = 0;

    virtual gsl::span<const float> GetColors() const = 0;

    virtual gsl::span<const float> GetTangents() const = 0;

    virtual gsl::span<const float> GetBitangents() const = 0;

}; // class IMeshData

class DUSK_CORE_API IMesh 
{
public:

    DISALLOW_COPY_AND_ASSIGN(IMesh)

    explicit IMesh() = default;

    virtual ~IMesh() = default;

    virtual void Render() = 0;

    virtual bool Load(const IMeshData * data) = 0;

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

}; // class IMesh

} // namespace Dusk

#endif // DUSK_CORE_MESH_HPP
#ifndef DUSK_MESH_DATA_HPP
#define DUSK_MESH_DATA_HPP

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
        LineStrip,
        Triangles,
        TriangleStrip,
    };

    DISALLOW_COPY_AND_ASSIGN(MeshData);

    MeshData() = default;

    virtual ~MeshData() = default;

    virtual Mode GetMode() const = 0;

    virtual gsl::span<const unsigned> GetIndices() const = 0;

    virtual gsl::span<const float> GetVertices() const = 0;

    virtual gsl::span<const float> GetNormals() const = 0;

    virtual gsl::span<const float> GetUVs() const = 0;

    virtual gsl::span<const float> GetColors() const = 0;

    virtual gsl::span<const float> GetTangents() const = 0;

    virtual gsl::span<const float> GetBitangents() const = 0;

}; // class MeshData

} // namespace Dusk

#endif // DUSK_MESH_DATA_HPP
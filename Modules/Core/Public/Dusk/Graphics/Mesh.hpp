#ifndef DUSK_CORE_MESH_HPP
#define DUSK_CORE_MESH_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>

#include <gsl/gsl>

namespace Dusk {

class DUSK_CORE_API IMeshData
{
public:

    DISALLOW_COPY_AND_ASSIGN(IMeshData);

    explicit IMeshData() = default;

    virtual ~IMeshData() = default;

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

}; // class IMesh

} // namespace Dusk

#endif // DUSK_CORE_MESH_HPP
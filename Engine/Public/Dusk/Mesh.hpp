#ifndef DUSK_CORE_MESH_HPP
#define DUSK_CORE_MESH_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/MeshData.hpp>

#include <vector>

namespace Dusk {

class DUSK_ENGINE_API Mesh : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(Mesh)

    Mesh() = default;

    virtual ~Mesh() = default;

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

std::vector<std::shared_ptr<Mesh>> LoadMeshListFromFile(const std::string& filename);

} // namespace Dusk

#endif // DUSK_CORE_MESH_HPP
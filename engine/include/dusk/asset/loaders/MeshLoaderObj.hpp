#ifndef DUSK_MESH_LOADER_OBJ_HPP
#define DUSK_MESH_LOADER_OBJ_HPP

#include <dusk/Config.hpp>
#include <dusk/asset/Mesh.hpp>

namespace dusk
{

class MeshLoaderObj : public IMeshLoader
{
public:

    DISALLOW_COPY_AND_ASSIGN(MeshLoaderObj)

    MeshLoaderObj() = default;
    virtual ~MeshLoaderObj() = default;

    virtual std::vector<std::string> GetExtensions() const override;

    virtual bool Load(Mesh * mesh, const std::string& filename) override;

private:



}; // class MeshLoaderObj

} // namespace dusk

#endif // DUSK_MESH_LOADER_OBJ_HPP

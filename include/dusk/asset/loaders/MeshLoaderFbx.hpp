#ifndef DUSK_MESH_LOADER_FBX_HPP
#define DUSK_MESH_LOADER_FBX_HPP

#include <dusk/Config.hpp>
#include <dusk/asset/Mesh.hpp>

namespace dusk
{

class MeshLoaderFbx : public IMeshLoader
{
public:

    DISALLOW_COPY_AND_ASSIGN(MeshLoaderFbx);

    MeshLoaderFbx() = default;
    virtual ~MeshLoaderFbx() = default;

    virtual std::vector<std::string> GetExtensions() const override;

    virtual bool Load(Mesh * mesh, const std::string& filename) override;

private:

};

} // namespace dusk

#endif // DUSK_MESH_LOADER_FBX_HPP

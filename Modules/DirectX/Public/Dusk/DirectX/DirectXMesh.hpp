#ifndef DUSK_DIRECTX_MESH_HPP
#define DUSK_DIRECTX_MESH_HPP

#include <Dusk/DirectX/DirectXConfig.hpp>

#include <Dusk/Mesh.hpp>

namespace Dusk {

class DUSK_DIRECTX_API DirectXMesh : public Mesh
{
public:

    DISALLOW_COPY_AND_ASSIGN(DirectXMesh)

    DirectXMesh() = default;

    virtual ~DirectXMesh() = default;

    void Render() override;

    bool Load(const MeshData * data) override;

private:
    
}; // class DirectXMesh

}; // namespace Dusk

#endif // DUSK_DIRECTX_MESH_HPP